
#include <system.h>
#include <multiboot.h>
#include <stdio.h>
#include <malloc.c>

// memset, sets a range of memory
#include <string.h>
/** MEMORY ALLOCATION **/
// a memory block
struct MemBlock_t {
	unsigned int		used : 1;
	unsigned int		size;
	struct MemBlock_t*	next;
};
typedef struct MemBlock_t MemBlock;

// the location of the end of the kernel's image
extern char end;

// the kernel heap
static MemBlock* kheap;

// global multiboot info pointer
extern multiboot_info_t* MultiBootHeader;

// kernel's malloc
void* kmalloc( unsigned int size )
{
	// blocks for use later
	MemBlock* mblk, *newblk;
	char* cblk;
	unsigned int size2;

	// we can't allocate nothing!
	if( size == 0 )
		return NULL;

	// the real size
	size2 = size + sizeof( MemBlock );

	// is there a heap?
	if( kheap == (MemBlock*) NULL )
	{
		// set it to the end of the image
		kheap = mblk = (MemBlock*) &end;

		// set it up properly
		mblk->used = 0;
		mblk->size = ( 4 * 1024 * 1024 *1024) - sizeof( MemBlock ) - (uint_t) &end; // we allocate within the full memory available
		mblk->next = (MemBlock*) NULL;
	}

	// try to find a free block
	for( mblk = kheap; mblk != (MemBlock*) NULL; mblk = mblk->next )
	{
		if( ( !mblk->used ) && ( size2 <= mblk->size ) )
			break;
	}

	// see if one was found
	if( mblk == (MemBlock*) NULL )
		return NULL;

	// set up the block
	cblk = (char*) mblk + size2;
	newblk = (MemBlock*) cblk;

	// point to a new empty block, and init it
	newblk->used = 0;
	newblk->size = mblk->size - size2;
	newblk->next = mblk->next;

	// point to old block and init it
	mblk->used = 1;
	mblk->size = size;
	mblk->next = newblk;

	// return the pointer to the memory
	cblk = (char*) mblk;
	return cblk + sizeof( MemBlock );
}

// kernel's free
void kfree( void* block )
{
	// blocks
	MemBlock* mblk;
	char* cblk;

	// get the real block address
	cblk = (char*) block - sizeof( MemBlock );

	// find this address in the list
	for( mblk = kheap; mblk != (MemBlock*) NULL; mblk = mblk->next )
	{
		if( mblk == (MemBlock*) cblk )
			break;
	}

	// bad heap pointer
	if( mblk == (MemBlock*) NULL )
	{
		return;
	}

	// free the block
	mblk->used = 0;

	// this is the hard part: unlinking this block
	for( mblk = kheap; mblk != (MemBlock*) NULL; mblk = mblk->next )
	{
		// find one that isn't used
		while( ( !mblk->used ) && ( mblk->next != (MemBlock*) NULL ) && ( !mblk->next->used ) )
		{
			// resize this block
			mblk->size = (int) mblk->next - (int) mblk + mblk->next->size;

			// merge with the next block
			mblk->next = mblk->next->next;
		}
	}
}

// kernel's realloc
void* krealloc( void* blk, unsigned int size )
{
	// blocks
	MemBlock* mblk;
	void* newblk;
	char* cblk;

	// check to see if the block we're given is valid
	if( blk == (MemBlock*) NULL )
		return kmalloc( size );

	// check for a valid size, if invalid free the block
	if( size == 0 )
		kfree( blk );

	// allocate the new block
	newblk = kmalloc( size );

	// check that it's valid
	if( newblk == (MemBlock*) NULL )
		return NULL;

	// point to the data in the old block
	cblk = (char*) blk - sizeof( MemBlock );
	mblk = (MemBlock*) cblk;

	// copy the data
	memcpy( newblk, blk, mblk->size );

	// release the old block
	kfree( blk );

	// return it
	return newblk;
}
