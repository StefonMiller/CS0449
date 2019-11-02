//Stefon Miller
//Cs449 Reciation 11570
//Project 3

#include <stdlib.h>		//Directives
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include "mymalloc.h"
#pragma pack(1)

struct Block *head = NULL;	//Head of linked list

void *my_malloc(int size)	//Malloc's size memory
{
	struct Block *temp = head;	//temporary node for calculation
	struct Block *best = NULL;	//Node resulting from best fit algorithm										
        while(temp!=NULL)		//If head is null, there is no best fitting node
        {
		//If temp is not occupied, it has adequate size, and either best is null or temp's size is smaller than size, assign a new best node to temp
                if(temp->occ == 0 && (temp->size >= size) && (best == NULL || temp -> size < best-> size))
                {
                        best = temp;
                	if(best -> size == size)
                	{
                        	break;		//If there is a perfect match, no need to continue traversal
                	}
		}
                temp = temp -> next;		//Traverse Linked list for new best node
        }

	temp = head;				//Reset temp
	if(best == NULL)			//If no best node found, allocate via sbrk
	{
		if(temp == NULL)		//If head is null, create a new head
		{
			head = sbrk(sizeof(struct Block) + size);	//Allocate room for head
               		head->occ = 1;					//Make head occupied
        	       	head->size = size + sizeof(struct Block);	//Put size of head into size field
       	        	head->prev = NULL;				//Head of LL, so no previous node(Not circularly linked)
                	head->next = NULL;				//No nodes after because this is the first node in the LL
			return (char *)head + sizeof(struct Block);	//Return the address of head + size of the struct, so our struct data won't be overwritten
		}
		while(temp->next != NULL)				//If head is not null, traverse linked list until the end
		{
			temp = temp->next;
		}
		temp -> next = sbrk(size + sizeof(struct Block));	//Allocate new node
		temp -> next -> prev = temp;				//Link new node to list
		temp -> next -> occ = 1;				//Fill in fields
		temp -> next -> size = size + sizeof(struct Block);	
		temp -> next -> next = NULL;				//Newest node, so next is null
		return (char *)temp->next + sizeof(struct Block);	//Return temp address + size of struct so data in struct is not overwritten
	}
	else								//If best is not null, determine if the best node can be split
	{
		if(best -> size > (size + sizeof(struct Block)))	//If the size of the best node is more than enough for a new node, begin splitting
		{
			char *test = (char *)best + size + sizeof(struct Block);	//Get address of splitting point
			temp = (struct Block *)test;					//Set temp to address of split
			temp -> next = (best -> next);					//Link temp to list
			best -> next -> prev = temp;					//Link temp to best
			best -> next = temp;
			temp -> prev = best;
			temp -> size = best -> size - (size + sizeof(struct Block));	//Resize best and temp
			best -> size = size + sizeof(struct Block);
			temp -> occ = 0;						//Fill in fields for temp and best
			best -> occ = 1;
			return (char *)best + sizeof(struct Block);			//Return address of best + size of struct
		}
		else									//If there is not enough room for a split, simply return best + size of struct
		{
			best -> occ = 1;
			best -> size = size + sizeof(struct Block);
			return (char *)best + sizeof(struct Block);
		}
	}
}
void my_free(void *pointer)								//Free a node pointed to by pointer
{
	struct Block *ptr;								//Node of address passed in
	ptr = pointer - sizeof(struct Block);						//Assign Node to address - size of struct because malloc returns the address of where the data starts, not the struct
	ptr->occ=0;									//Unoccupy the node
	if(ptr->next == NULL)								//If the pointer is the last in the list(touching brk), execute a negative sbrk
	{	
		int size = ptr -> size;							//size counter for coalescing purposes
		if(ptr->prev!= NULL)							//If there is a pointer to a previous node, check if it is occupied
		{
			while(ptr->prev != NULL && ptr->prev->occ == 0)			//Loop until a previous node is occupied, and tally up the space of all free adjacent blocks
			{	
				size += ptr->prev->size;				//Sum up sizes
				ptr = ptr->prev;					//Traverse list backwards
				ptr->next = NULL;					//After every traversal backwards, remove the reference to the last node
			}
			if(ptr->prev != NULL)						//If there is one more node available, dereference the current node
			{
				ptr->prev->next = NULL;	
			}
		}
		sbrk(-1*(size));							//Reduce heap by size
		if(head == sbrk(0))							//If head and brk are equal, the list is empty
		{
			head = NULL;
		}
		return;									//Return from function once node is freed
	}
	
	if(ptr->prev == NULL)								//If there is no previous node, check if you can combine nodes to the right
	{
		while(ptr->next != NULL && ptr->next->occ == 0)				//If there are nodes to the right and they are unoccupied, coallesce them
		{
			ptr->size += ptr->next->size;					//Add size of next node to ptr
			ptr->next->next->prev=ptr;					//Combine nodes and link them appropriately
			ptr->next=ptr->next->next;				
		}
		return;									//Return once done
	}

	if((ptr->prev->occ == 0) && (ptr->next->occ == 0))				//If there are open nodes to the left and right, coallesce them both
	{
		ptr = ptr->prev;							//Set ptr to the leftmost node
		ptr->size = (ptr->size + ptr->next->size + ptr->next->next->size);	//Set size of ptr to summation of all 3
		ptr->next->next->next->prev = ptr;					//Link ptr to list appropriately	
		ptr->next = ptr->next->next->next;				
	}
	else if(ptr->prev->occ == 0)							//If there is only an unoccupied node to the left, coallesce left
	{
		ptr->prev->size = (ptr->size + ptr->prev->size);			//Combine sizes
		ptr->prev->next = ptr->next;						//Link to list appropriately
		ptr->next->prev = ptr->prev;
		ptr = ptr->prev;							//Move ptr to the lefmost node
	}	
	else if(ptr->next->occ == 0)							//If there is only an unoccupied node to the right, coallesce rigth
	{
		ptr->size = (ptr->size + ptr->next->size);				//Set ptr->size to total size of both nodes
		ptr->next->next->prev=ptr;
		ptr->next = ptr->next->next;						//Link ptr to list appropriately, no need to move ptr as it is the leftmost node
	}
	else										//If no adjacent nodes are open, no need to coallesce
	{
	}
	
}
void dump_heap()									//Dump_heap() function given in lab
{
  struct Block *cur;
  printf("brk: %p\n", sbrk(0));
  printf("head->");
  for(cur = head; cur != NULL; cur = cur->next) 
  {
    printf("[%d:%d:%d]->", cur->occ, (char*)cur - (char*)head, cur->size);
    fflush(stdout);
    assert((char*)cur >= (char*)head && (char*)cur + cur->size <= (char*)sbrk(0)); // check t$
    if(cur->next != NULL) {
      assert(cur->next->prev == cur); // if not last block, check that forward/backward links$
      assert((char*)cur + cur->size == (char*)cur->next); // check that the block size is cor$
    }
  }
  printf("NULL\n");
}
