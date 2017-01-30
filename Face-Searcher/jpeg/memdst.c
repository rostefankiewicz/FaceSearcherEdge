#include <stdio.h>
#include "jpeglib.h"
//#include "memdst.h"


/*
This a custom destination manager for jpeglib that
enables the use of memory to memory compression.
See IJG documentation for details.
*/


typedef struct
{
  struct jpeg_destination_mgr pub; /* base class */
	JOCTET* buffer; /* buffer start address */
	int bufsize; /* size of buffer */
	size_t datasize; /* final size of compressed data */
	int* outsize; /* user pointer to datasize */
	int errcount; /* counts up write errors due to buffer overruns */
} memory_destination_mgr;

typedef memory_destination_mgr* mem_dest_ptr;

/* This function is called by the library before any data gets written */



METHODDEF(void) init_destination (j_compress_ptr cinfo)
{
	mem_dest_ptr dest = (mem_dest_ptr)cinfo->dest;
	dest->pub.next_output_byte = dest->buffer; /* set destination buffer */
	dest->pub.free_in_buffer = dest->bufsize; /* input buffer size */
	dest->datasize = 0; /* reset output size */
	dest->errcount = 0; /* reset error count */
}


/* 
	This function is called by the library if the buffer fills up
	just reset destination pointer and buffer size here.
	The application should allocate enough memory to hold the entire image, so if
	this function is ever called, it is a buffer overrun error.
*/
METHODDEF(boolean) empty_output_buffer (j_compress_ptr cinfo)
{
  mem_dest_ptr dest = (mem_dest_ptr)cinfo->dest;
  dest->pub.next_output_byte = dest->buffer;
  dest->pub.free_in_buffer = dest->bufsize;
  ++dest->errcount; /* need to increase error count */
	
	return TRUE;
}

/*
	Usually the library wants to flush output here. calculate output buffer size here.
*/
METHODDEF(void) term_destination (j_compress_ptr cinfo)
{
  mem_dest_ptr dest = (mem_dest_ptr)cinfo->dest;
  dest->datasize = dest->bufsize - dest->pub.free_in_buffer;
  
	if (dest->outsize)
  {
    if (dest->errcount)   *dest->outsize = -1;
    else									*dest->outsize = (int)dest->datasize;
  }
}

/*	Override the default destination manager initialization
		provided by jpeglib. Since we want to use memory-to-memory
		compression, we need to use our own destination manager.
*/
GLOBAL(void) jpeg_memory_dest( j_compress_ptr cinfo, JOCTET* buffer, int bufsize, int* outsize)
{
  mem_dest_ptr dest;

  /* first call for this instance - need to setup */
  if (cinfo->dest == 0)
	{
    cinfo->dest = (struct jpeg_destination_mgr *)
    (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
    sizeof (memory_destination_mgr));
  }
	
  dest = (mem_dest_ptr) cinfo->dest;
  dest->bufsize = bufsize;
  dest->buffer = buffer;
  dest->outsize = outsize;
  
	/* set method callbacks */
  dest->pub.init_destination = init_destination;
  dest->pub.empty_output_buffer = empty_output_buffer;
  dest->pub.term_destination = term_destination;
}
