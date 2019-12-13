#include <stdio.h>
#include <stdlib.h>
#include "Aritmetico.h"
#include <math.h>

/*****************************************************************************/
/*                       ARITHMETIC ENCODING FUNCTIONS.	                     */
/*****************************************************************************/
/* START ENCODING A STREAM OF SYMBOLS */

ArithmeticCoder :: ArithmeticCoder()
{  
 low = 0;				/* Full code range */
 high = Top_value;
 bits_to_follow = 0;			/* No bits to follow next */
 //printf("high = %u\n", high);
 //printf("low = %u\n", low);
 //printf("Code_value_bits = %u\n", Code_value_bits);
 //printf("Top_value = %u\n", Top_value);
 //printf("First_qtr = %u\n", First_qtr);
 //printf("Half = %u\n", Half);
 //printf("Third_qtr = %u\n", Third_qtr);

/* INITIALISE BIT OUTPUT */
 buffer = 0;		   /* Bits buffered for ouput		*/
 bits_to_go = 8;	   /* Number of bits free in buffer	*/
 //Bits_Count = 0;

 //Probabilistic models
 Nmodels = 0;
 model = NULL;

 temp_model.cum_freq = NULL;
 temp_model.freq = NULL;
 temp_model.log2freq = NULL;
 temp_model.char_to_index = NULL;
 temp_model.index_to_char = NULL;
}

void ArithmeticCoder :: InitCoder(void)
{

 int m;
 
 low = 0;				/* Full code range */
 high = Top_value;
 bits_to_follow = 0;			/* No bits to follow next */
 //printf("high = %u\n", high);
 //printf("low = %u\n", low);
 //printf("Code_value_bits = %u\n", Code_value_bits);
 //printf("Top_value = %u\n", Top_value);
 //printf("First_qtr = %u\n", First_qtr);
 //printf("Half = %u\n", Half);
 //printf("Third_qtr = %u\n", Third_qtr);

/* INITIALISE BIT OUTPUT */
 buffer = 0;		   /* Bits buffered for ouput		*/
 bits_to_go = 8;	   /* Number of bits free in buffer	*/
 //Bits_Count = 0;

 //Probabilistic models
 for(m = 0; m < Nmodels; m++) {

   //Memory allocation
   if(model[m].cum_freq != NULL) free(model[m].cum_freq);
   if(model[m].freq != NULL) free(model[m].log2freq);
   if(model[m].log2freq != NULL) free(model[m].freq);
   if(model[m].char_to_index != NULL) free(model[m].char_to_index);
   if(model[m].index_to_char != NULL) free(model[m].index_to_char);
 }

 Nmodels = 0;
 if(model != NULL) free(model);
 model = NULL;

 temp_model.cum_freq = NULL;
 temp_model.freq = NULL;
 temp_model.log2freq = NULL;
 temp_model.char_to_index = NULL;
 temp_model.index_to_char = NULL;
}

void ArithmeticCoder :: set_output_file(char *nome) {

  if((ofp = fopen(nome, "wb")) == NULL) {
    printf("Unable to open output file\n");
    exit(0);
  }
}

/* ENCODE A SYMBOL */

void ArithmeticCoder :: encode_symbol(int symbol, int m) {
  //symbol: Symbol to encode
  //model: Cummulative symbol frequencies 

 //unsigned long long range;	/* size of the current_code region	*/
 long long range;	/* size of the current_code region	*/

 symbol = model[m].char_to_index[symbol];

 //range = (unsigned long long)(high-low)+1;		/* Narrow the code  */
 range = (long long)(high-low)+1;		/* Narrow the code  */

 //high = low + ((unsigned long long)range*model[m].cum_freq[symbol-1])/model[m].cum_freq[0]-1; /* region to that   */
 //low  = low + ((unsigned long long)range*model[m].cum_freq[symbol])/model[m].cum_freq[0];     /* allotted to this */
 high = low + (range*model[m].cum_freq[symbol-1])/model[m].cum_freq[0]-1; /* region to that   */
 low  = low + (range*model[m].cum_freq[symbol])/model[m].cum_freq[0];     /* allotted to this */
													       /* symbol           */

 for (;;) 
    {			       /* Loop to output bits	*/
     if (high<Half) 
		{
         bit_plus_follow(0);   /* Output 0 if in low half */
        }
     else if (low>=Half) 
			{      /* Output 1 if in high half	*/
             bit_plus_follow(1);
             low = low-Half;
             high = high-Half;     /* Subtract offset to top */
            }
          else if (low>=First_qtr && high<Third_qtr)       
				{		   						/* Output an opposite bit   */
				           						/* later if in middle half.	*/
                 bits_to_follow += 1;
                 low = low-First_qtr;	 /* Subtract offset to middle */
		         high = high-First_qtr;
                 }
               else break;	       /* Otherwise exit loop */
          low = 2*low;
          high = 2*high+1;	       /* Scale up code range */
    }
}

/* ENCODE A SYMBOL USING TEMPORARY MODEL*/

void ArithmeticCoder :: encode_symbol_tm(int symbol) {
  //symbol: Symbol to encode
  //temp_model: Cummulative symbol frequencies 

 //unsigned long long range;	/* size of the current_code region	*/
 long long range;	/* size of the current_code region	*/

 symbol = temp_model.char_to_index[symbol];

 //range = (unsigned long long)(high-low)+1;		/* Narrow the code  */
 range = (long long)(high-low)+1;		/* Narrow the code  */

 //high = low + ((unsigned long long)range*model[m].cum_freq[symbol-1])/model[m].cum_freq[0]-1; /* region to that   */
 //low  = low + ((unsigned long long)range*model[m].cum_freq[symbol])/model[m].cum_freq[0];     /* allotted to this */
 high = low + (range*temp_model.cum_freq[symbol-1])/temp_model.cum_freq[0]-1; /* region to that   */
 low  = low + (range*temp_model.cum_freq[symbol])/temp_model.cum_freq[0];     /* allotted to this */
													       /* symbol           */

 for (;;) 
    {			       /* Loop to output bits	*/
     if (high<Half) 
		{
         bit_plus_follow(0);   /* Output 0 if in low half */
        }
     else if (low>=Half) 
			{      /* Output 1 if in high half	*/
             bit_plus_follow(1);
             low = low-Half;
             high = high-Half;     /* Subtract offset to top */
            }
          else if (low>=First_qtr && high<Third_qtr)       
				{		   						/* Output an opposite bit   */
				           						/* later if in middle half.	*/
                 bits_to_follow += 1;
                 low = low-First_qtr;	 /* Subtract offset to middle */
		         high = high-First_qtr;
                 }
               else break;	       /* Otherwise exit loop */
          low = 2*low;
          high = 2*high+1;	       /* Scale up code range */
    }
}
/* FINISH ENCODING THE STREAM */

void ArithmeticCoder :: done_encoding(void)
{ 
 bits_to_follow += 1;		        /* Output two bits that 	*/
 if (low<First_qtr) bit_plus_follow(0);	/* select the quarter that 	*/
 else bit_plus_follow(1);		/* the current code range       */
					/* contains	                */
 done_outputing_bits();
}

/* OUTPUT BITS PLUS THE FOLLOWING OPPOSITE BITS */

void ArithmeticCoder :: bit_plus_follow(int bit)
{  
 output_bit(bit);		 /* Output the bit				*/
 while (bits_to_follow>0) 
	{
     output_bit(!bit);	         /* Ouput bits_to_follow   */
     bits_to_follow -= 1;	 /* opposite bits. Set			*/
    }				 /* bits_to_follow to zero		*/
}
            
/* BIT OUTPUT ROUTINES */

/* OUTPUT A BIT  */

void ArithmeticCoder :: output_bit(int bit)
{ 
 buffer >>= 1;				/* Put bit in top of buffer	*/
 if (bit) buffer |= 0x80;
 bits_to_go -= 1;
 //Bits_Count += 1;

 if (bits_to_go == 0) 		/* Output buffer if it is	*/
 {			/* now full			*/
   //fwrite(&buffer,1, 1,ofp);
   fprintf(ofp, "%c", buffer);
   bits_to_go = 8;
 }
}


/* FLUSH OUT THE LAST BITS */

void ArithmeticCoder :: done_outputing_bits(void)
{ 
 buffer = buffer>>bits_to_go;
 //fwrite(&buffer,1,1,ofp);
 fprintf(ofp, "%c", buffer);
 fclose(ofp);
}

/*****************************************************************************/
/*                       ARITHMETIC DECODING FUNCTIONS.	                     */
/*****************************************************************************/
ArithmeticDecoder :: ArithmeticDecoder(void) {
/* START DECODING A STREAM OF SYMBOLS */ 
 low = 0;			     /* full code range   		*/
 high = Top_value;

 //printf("high = %u\n", high);
 //printf("low = %u\n", low);
 //printf("Code_value_bits = %u\n", Code_value_bits);
 //printf("Top_value = %u\n", Top_value);
 //printf("First_qtr = %u\n", First_qtr);
 //printf("Half = %u\n", Half);
 //printf("Third_qtr = %u\n", Third_qtr);


 bits_to_go = 0;	/* Buffer starts out with		*/
 garbage_bits = 0;	/* no bits in it.			*/
 //Input_Bits_Count=0;

 //Probabilistic models
 Nmodels = 0;
 model = NULL;

 temp_model.cum_freq = NULL;
 temp_model.freq = NULL;
 temp_model.char_to_index = NULL;
 temp_model.index_to_char = NULL;
}

void ArithmeticDecoder :: set_input_file(char *nome) {
/* START DECODING A STREAM OF SYMBOLS */ 
 int i;

  if((ifp = fopen(nome, "rb")) == NULL) {
    printf("Unable to open input file\n");
    exit(0);
  }

 value = 0;				               
 for (i=1; i<=Code_value_bits; i++) 	
 {			     	     /* input bits to fill the 		*/
     value = 2*value+input_bit();    /* code value    			*/
 }
 low = 0;			     /* full code range   		*/
 high = Top_value;


}
void ArithmeticDecoder :: done_decoding(void) {
  fclose(ifp);
}

/* DECODE THE NEXT SYMBOL */
int ArithmeticDecoder :: decode_symbol(int m)
{ 
 //unsigned long long range;		/* size of surrent code region		*/
 //unsigned long int cum;
 long long range;		/* size of surrent code region		*/
 long int cum;	    /* cummulative frequency calculated	*/

 int symbol;				/* symbol decoded			*/

 //cum_freq = model[m].cum_freq;
 //range = (unsigned long long)(high-low)+1;
 //cum = (((unsigned long long int)(value-low)+1)*model[m].cum_freq[0]-1)/range; /* Find cumfreq for value */
 range = (long long)(high-low)+1;
 cum = (((long long)(value-low)+1)*model[m].cum_freq[0]-1)/range; /* Find cumfreq for value */

 for (symbol=1;  model[m].cum_freq[symbol]>cum; symbol++) ;   /* Then find symbol	   */


 //high = low + ((unsigned long long)range*model[m].cum_freq[symbol-1])/model[m].cum_freq[0]-1; 
 //low  = low + ((unsigned long long)range*model[m].cum_freq[symbol])/model[m].cum_freq[0];    
 high = low + (range*model[m].cum_freq[symbol-1])/model[m].cum_freq[0]-1; 
 low  = low + (range*model[m].cum_freq[symbol])/model[m].cum_freq[0];    

 for (;;)	            	/* Loop to get rid of bits	*/
    {
     if (high<Half) 
	{
        }
     else if (low>=Half) 
        {	     			/* Expand high half */
           value = value-Half;
           low = low-Half;		 /* Subtract offset to top */
           high = high-Half;
         }
         else if (low>=First_qtr && high<Third_qtr) 
        	{								/* Expand middle half		 */
                 value = value-First_qtr;
                 low = low-First_qtr;/* Subtract offset to middle*/
                 high = high-First_qtr;
                }
               else break;	/* Otherwise exit loop */
     low = 2*low;
     high = 2*high+1;		/* Scale up code range		 */
     value = 2*value+input_bit();	/* Move in next input bit	 */
    }
 
 return(model[m].index_to_char[symbol]);
}

/* DECODE THE NEXT SYMBOL USING TEMPORARY MODEL*/
int ArithmeticDecoder :: decode_symbol_tm(void)
{ 
 //unsigned long long range;		/* size of surrent code region		*/
 //unsigned long int cum;
 long long range;		/* size of surrent code region		*/
 long int cum;	    /* cummulative frequency calculated	*/

 int symbol;				/* symbol decoded			*/

 //cum_freq = model[m].cum_freq;
 //range = (unsigned long long)(high-low)+1;
 //cum = (((unsigned long long int)(value-low)+1)*model[m].cum_freq[0]-1)/range; /* Find cumfreq for value */
 range = (long long)(high-low)+1;
 cum = (((long long)(value-low)+1)*temp_model.cum_freq[0]-1)/range; /* Find cumfreq for value */

 for (symbol=1;  temp_model.cum_freq[symbol]>cum; symbol++) ;   /* Then find symbol	   */


 //high = low + ((unsigned long long)range*model[m].cum_freq[symbol-1])/model[m].cum_freq[0]-1; 
 //low  = low + ((unsigned long long)range*model[m].cum_freq[symbol])/model[m].cum_freq[0];    
 high = low + (range*temp_model.cum_freq[symbol-1])/temp_model.cum_freq[0]-1; 
 low  = low + (range*temp_model.cum_freq[symbol])/temp_model.cum_freq[0];    

 for (;;)	            	/* Loop to get rid of bits	*/
    {
     if (high<Half) 
	{
        }
     else if (low>=Half) 
        {	     			/* Expand high half */
           value = value-Half;
           low = low-Half;		 /* Subtract offset to top */
           high = high-Half;
         }
         else if (low>=First_qtr && high<Third_qtr) 
        	{								/* Expand middle half		 */
                 value = value-First_qtr;
                 low = low-First_qtr;/* Subtract offset to middle*/
                 high = high-First_qtr;
                }
               else break;	/* Otherwise exit loop */
     low = 2*low;
     high = 2*high+1;		/* Scale up code range		 */
     value = 2*value+input_bit();	/* Move in next input bit	 */
    }
 return(temp_model.index_to_char[symbol]);
}
/* INPUT A BIT  */
int ArithmeticDecoder :: input_bit(void)
{ 
 int t;

 if (bits_to_go==0)          		/* read the next byte if no */
 {  
   //fread(&buffer, 1, 1, ifp);
   buffer = fgetc(ifp);
    bits_to_go = 8;
 }

 t = buffer&1;									/* Return the next bit from	*/

 buffer >>= 1;									/* the bottom of the byte 	*/

 bits_to_go -= 1;

 //Input_Bits_Count++;

 return(t);

}

/*****************************************************************************/
/*                    ADAPTIVE MODEL FUNCTIONS (ENCODER).	             */
/*****************************************************************************/

/* INITIALISE THE MODEL */
void ArithmeticCoder :: start_model(int n, int m)
{  
 int i;
 
 model[m].no_of_char = n;

 //Memory allocation
 if((model[m].cum_freq = (unsigned long int *) malloc((n+1)*sizeof(unsigned long int)))==NULL) {
   printf("Memory allocation error\n");
   exit(0);
 }
 if((model[m].freq = (unsigned long int *) malloc((n+1)*sizeof(unsigned long int)))==NULL) {
   printf("Memory allocation error\n");
   exit(0);
 }
 if((model[m].char_to_index = (int *) malloc((n+1)*sizeof(int)))==NULL) {
   printf("Memory allocation error\n");
   exit(0);
 }
 if((model[m].index_to_char = (int *) malloc((n+1)*sizeof(int)))==NULL) {
   printf("Memory allocation error\n");
   exit(0);
 }
 if((model[m].log2freq = (float *) malloc((n+1)*sizeof(float)))==NULL) {
   printf("Memory allocation error\n");
   exit(0);
 }

 for (i=0; i< n; i++)
 { 			         /* Set up tables that  */
     model[m].char_to_index[i] = i+1;     /* translate between symbol */
     model[m].index_to_char[i+1] = i;     /* indexes and characters */ 
 }

 model[m].cum_freq[n] = 0;
 model[m].freq[0] = 0;
 
 for (i=n; i>0;i--) 
 {           		 		/* Set up initial frequency */
     model[m].freq[i] = 1;        		/* counts to be one for all */
     model[m].cum_freq[i-1] = model[m].cum_freq[i] + model[m].freq[i] ;    	/* symbols     */
     model[m].log2freq[i] = 0;        		
 }

 model[m].log2cum = log(1.0*model[m].cum_freq[0])/log(2);
 //model[m].freq[0]=0;          /* freq[0] must not be the */
    	                     /* same as freq[1]   */
}

void ArithmeticCoder :: start_pmodel(int n, PModel &external_model)
{  
 int i;
 
 external_model.no_of_char = n;

 //Memory allocation
 if((external_model.cum_freq = (unsigned long int *) malloc((n+1)*sizeof(unsigned long int)))==NULL) {
   printf("Memory allocation error\n");
   exit(0);
 }
 if((external_model.freq = (unsigned long int *) malloc((n+1)*sizeof(unsigned long int)))==NULL) {
   printf("Memory allocation error\n");
   exit(0);
 }
 if((external_model.char_to_index = (int *) malloc((n+1)*sizeof(int)))==NULL) {
   printf("Memory allocation error\n");
   exit(0);
 }
 if((external_model.index_to_char = (int *) malloc((n+1)*sizeof(int)))==NULL) {
   printf("Memory allocation error\n");
   exit(0);
 }
 if((external_model.log2freq = (float *) malloc((n+1)*sizeof(float)))==NULL) {
   printf("Memory allocation error\n");
   exit(0);
 }

 for (i=0; i< n; i++)
 { 			         /* Set up tables that  */
     external_model.char_to_index[i] = i+1;     /* translate between symbol */
     external_model.index_to_char[i+1] = i;     /* indexes and characters */ 
 }

 external_model.cum_freq[n] = 0;
 external_model.freq[0] = 0;
 
 for (i=n; i>0;i--) 
 {           		 		/* Set up initial frequency */
     external_model.freq[i] = 1;        		/* counts to be one for all */
     external_model.cum_freq[i-1] = external_model.cum_freq[i] + external_model.freq[i] ;    	/* symbols     */
     external_model.log2freq[i] = 0;        		
 }

 external_model.log2cum = log(1.0*external_model.cum_freq[0])/log(2);
 //model[m].freq[0]=0;          /* freq[0] must not be the */
    	                     /* same as freq[1]   */
}

void ArithmeticCoder :: delete_pmodel(PModel &external_model)
{  

 //Memory deallocation
 free(external_model.cum_freq);
 free(external_model.freq);
 free(external_model.char_to_index);
 free(external_model.index_to_char);
 free(external_model.log2freq);
    	                     
}

void ArithmeticCoder :: restart_model(int m)
{  
 int i;
 
 int n=model[m].no_of_char;
 
 for (i=0; i< n; i++)
 { 			         /* Set up tables that  */
     model[m].char_to_index[i] = i+1;     /* translate between symbol */
     model[m].index_to_char[i+1] = i;     /* indexes and characters */ 
 }

 model[m].cum_freq[n] = 0;
 model[m].freq[0] = 0;
 
 for (i=n; i>0;i--) 
 {           		 		/* Set up initial frequency */
     model[m].freq[i] = 1;        		/* counts to be one for all */
     model[m].cum_freq[i-1] = model[m].cum_freq[i] + model[m].freq[i] ;    	/* symbols     */
     model[m].log2freq[i] = 0;        		
 }

 model[m].log2cum = log(1.0*model[m].cum_freq[0])/log(2);
 //model[m].freq[0]=0;          /* freq[0] must not be the */
    	                     /* same as freq[1]   */
}

/*  Includes a new char in model m   */
void ArithmeticCoder :: add_new_char(int m)
{
 
    int i;
    unsigned long int cum;					

   i = model[m].no_of_char;
 //Memory allocation
 if((model[m].cum_freq = (unsigned long int *) realloc(model[m].cum_freq, (i+2)*sizeof(unsigned long int)))==NULL) {
   printf("Memory allocation error\n");
   exit(0);
 }
 if((model[m].freq = (unsigned long int *) realloc(model[m].freq, (i+2)*sizeof(unsigned long int)))==NULL) {
   printf("Memory allocation error\n");
   exit(0);
 }
 if((model[m].char_to_index = (int *) realloc(model[m].char_to_index, (i+2)*sizeof(int)))==NULL) {
   printf("Memory allocation error\n");
   exit(0);
 }
 if((model[m].index_to_char = (int *) realloc(model[m].index_to_char, (i+2)*sizeof(int)))==NULL) {
   printf("Memory allocation error\n");
   exit(0);
 }
 if((model[m].log2freq = (float *) realloc(model[m].log2freq, (i+2)*sizeof(float)))==NULL) {
   printf("Memory allocation error\n");
   exit(0);
 }

 if (model[m].cum_freq[0]==Max_freq) 		/* See if frequency counts	*/
	{					/* are at their maximum		*/
     cum = 0;

     for (i = model[m].no_of_char; i>=0; i--) 	/* If so, halve all the 	*/
		{				/* counts ( keeping them	*/
         model[m].freq[i] = (model[m].freq[i]+1)/2;		/* non-zero).			*/
         model[m].log2freq[i] = log(1.0*model[m].freq[i])/log(2);
         model[m].cum_freq[i] = cum;						
         cum += model[m].freq[i];
        }
    }
    model[m].freq[0] = 0;

  i = model[m].no_of_char;

   model[m].char_to_index[i]  = i+1;
   model[m].index_to_char[i+1]= i;
   model[m].freq[i+1] = 1;
   model[m].log2freq[i+1] = 0;
   model[m].cum_freq[i]  = 1;
   model[m].cum_freq[i+1]= 0;
   
   
   while(i)
   {
      i--;
      model[m].cum_freq[i]++;
   }
   (model[m].no_of_char)++;   
   
   model[m].log2cum = log(1.0*model[m].cum_freq[0])/log(2);

}

/* UPDATE THE MODEL TO ACCOUNT FOR A NEW SYMBOL */
void ArithmeticCoder :: update_model(int symbol, int m)
{ 
 int i;                  /* New index for symbol  */
 unsigned long int cum;					

 symbol = model[m].char_to_index[symbol];

 if (model[m].cum_freq[0]==Max_freq) 		/* See if frequency counts	*/
	{					/* are at their maximum		*/
     cum = 0;

     for (i = model[m].no_of_char; i>=0; i--) 	/* If so, halve all the 	*/
		{				/* counts ( keeping them	*/
         model[m].freq[i] = (model[m].freq[i]+1)/2;		/* non-zero).	*/
         model[m].log2freq[i] = log(1.0*model[m].freq[i])/log(2);
         model[m].cum_freq[i] = cum;						
         cum += model[m].freq[i];
      }
    }
    model[m].freq[0] = 0;

 /* Find symbol's new index */
 for (i = symbol; model[m].freq[i]==model[m].freq[i-1]; i--) ;
 if (i<symbol) 
 {
     int ch_i, ch_symbol;

     ch_i = model[m].index_to_char[i];     /* Update the translation  */
     ch_symbol = model[m].index_to_char[symbol];   /* tables if the symbol has */
     model[m].index_to_char[i] = ch_symbol;    /* moved     */
     model[m].index_to_char[symbol] = ch_i;
     model[m].char_to_index[ch_i] = symbol;
     model[m].char_to_index[ch_symbol] = i;
  }

  model[m].freq[i] += 1;
  model[m].log2freq[i] = log(1.0*model[m].freq[i])/log(2);

  while (i>0)
  {
    i--;
    model[m].cum_freq[i]++;
  }

  model[m].log2cum = log(1.0*model[m].cum_freq[0])/log(2);
  
}

void ArithmeticCoder :: load_model(int m, PModel &source_model)
{ 
    int n;
    
    if(model[m].no_of_char != source_model.no_of_char) {
        printf("Erro: cannot load from a PModel of different size");
        exit(0);
    }
    
    for(n = 0; n <= model[m].no_of_char; n++) {
        model[m].index_to_char[n] = source_model.index_to_char[n];
        model[m].char_to_index[n] = source_model.char_to_index[n];
        model[m].freq[n] = source_model.freq[n];
        model[m].cum_freq[n] = source_model.cum_freq[n];
        model[m].log2freq[n] = source_model.log2freq[n];
        model[m].log2cum = source_model.log2cum;
        
    }
}

void ArithmeticCoder :: get_model(int m, PModel &target_model)
{ 
 
    int n;
    
    if(model[m].no_of_char != target_model.no_of_char) {
        printf("Erro: cannot store to a PModel of different size");
        exit(0);
    }
    
    for(n = 0; n <= model[m].no_of_char; n++) {
    
        target_model.index_to_char[n] = model[m].index_to_char[n];
        target_model.char_to_index[n] = model[m].char_to_index[n];
        target_model.freq[n] = model[m].freq[n];
        target_model.cum_freq[n] = model[m].cum_freq[n];
        target_model.log2freq[n] = model[m].log2freq[n];
        target_model.log2cum = model[m].log2cum;
        
    }
  
}

void ArithmeticCoder :: print_model(int m) {
  int n;
  printf("Freq[%d] = [", m); 
  for(n = 0; n <= model[m].no_of_char; n++) {
    printf(" %d", model[m].freq[n]);
  }
  printf("]\n"); 
  
}

void ArithmeticCoder :: add_model(void) {

  Nmodels++;
  if(model == NULL) {
    if((model = (PModel *) malloc(Nmodels*sizeof(PModel))) == NULL) {
      printf("Memory allocation error\n");
      exit(0);
    }
  }
  else {
    if((model = (PModel *) realloc(model, Nmodels*sizeof(PModel))) == NULL) {
      printf("Memory allocation error\n");
      exit(0);
    }
  }
  model[Nmodels-1].no_of_char = 0;
}

float ArithmeticCoder :: rate(int symbol, int m) {

//  float r;

  symbol = model[m].char_to_index[symbol];
//  r = model[m].freq[symbol];
//  r = r/model[m].cum_freq[0];
//  r = -log(r)/log(2);
//  return(r);
  return(model[m].log2cum-model[m].log2freq[symbol]);
}

float ArithmeticCoder :: entropy(int m) {

  float p, H;
  int n, symbol;

  H = 0;
  for(n = 0; n < model[m].no_of_char; n++) {
  	symbol = model[m].char_to_index[n];
  	p = model[m].freq[symbol];
  	p = p/model[m].cum_freq[0];
  	H = H -p*log(p)/log(2);
  }
  return(H);
}

void ArithmeticCoder :: copy_model_mask(int m, int *mask) {

 int n, k;

 n = model[m].no_of_char;
 if(temp_model.cum_freq != NULL) free(temp_model.cum_freq);
 if(temp_model.freq != NULL) free(temp_model.freq);
 if(temp_model.char_to_index != NULL) free(temp_model.char_to_index);
 if(temp_model.index_to_char != NULL) free(temp_model.index_to_char);
 if(temp_model.log2freq != NULL) free(temp_model.log2freq);
 //Memory allocation
 if((temp_model.cum_freq = (unsigned long int *) malloc((n+1)*sizeof(unsigned long int)))==NULL) {
   printf("Memory allocation error\n");
   exit(0);
 }
 if((temp_model.freq = (unsigned long int *) malloc((n+1)*sizeof(unsigned long int)))==NULL) {
   printf("Memory allocation error\n");
   exit(0);
 }
 if((temp_model.char_to_index = (int *) malloc((n+1)*sizeof(int)))==NULL) {
   printf("Memory allocation error\n");
   exit(0);
 }
 if((temp_model.index_to_char = (int *) malloc((n+1)*sizeof(int)))==NULL) {
   printf("Memory allocation error\n");
   exit(0);
 }
 if((temp_model.log2freq = (float *) malloc((n+1)*sizeof(float)))==NULL) {
   printf("Memory allocation error\n");
   exit(0);
 }

 k = 1;
 for(n = 0; n < model[m].no_of_char; n++) {
  if(mask[n] == 1) {
    temp_model.char_to_index[n] = k;
    temp_model.index_to_char[k] = n;
    k++;
  }
 } 
 temp_model.no_of_char = k-1;
 for(n = 0; n < temp_model.no_of_char; n++) {
  //temp_model.freq[n+1] = model[m].freq[model[m].char_to_index[n]];
  //temp_model.log2freq[n+1] = model[m].log2freq[model[m].char_to_index[n]];
  temp_model.freq[n+1] = model[m].freq[model[m].char_to_index[temp_model.index_to_char[n+1]]];
  temp_model.log2freq[n+1] = model[m].log2freq[model[m].char_to_index[temp_model.index_to_char[n+1]]];
 }
 temp_model.freq[0] = 0; 
 temp_model.cum_freq[temp_model.no_of_char] = 0; 
 for(n = temp_model.no_of_char; n > 0; n--) {
  temp_model.cum_freq[n-1] = temp_model.cum_freq[n] + temp_model.freq[n];
 }
 temp_model.log2cum = log(temp_model.cum_freq[0])/log(2);
}

void ArithmeticCoder :: copy_model(int m, int n) {

    int k;
  if(model[m].no_of_char != model[m].no_of_char) {
      printf("Error: cannot copy arithmetic models of different sizes\n");
      return;
  }  
  
  for(k = 0; k <= model[m].no_of_char; k++) {

    model[m].index_to_char[k] = model[n].index_to_char[k];
    model[m].char_to_index[k] = model[n].char_to_index[k];
    model[m].freq[k] = model[n].freq[k];
    model[m].cum_freq[k] = model[n].cum_freq[k];
    model[m].log2freq[k] = model[n].log2freq[k];
    model[m].log2cum = model[n].log2cum;
    
  }

}

float ArithmeticCoder :: rate_tm(int symbol) {

  float r;

  symbol = temp_model.char_to_index[symbol];
//  r = temp_model.freq[symbol];
//  r = r/temp_model.cum_freq[0];
//  r = -log(r)/log(2);
//  return(r);
  return(temp_model.log2cum - temp_model.log2freq[symbol]);
}

/*****************************************************************************/
/*                    ADAPTIVE MODEL FUNCTIONS (DECODER).	             */
/*****************************************************************************/

/* INITIALISE THE MODEL */
void ArithmeticDecoder :: start_model(int n, int m)
{ 

 int i;
 
 model[m].no_of_char = n;

 //Memory allocation
 if((model[m].cum_freq = (unsigned long int *) malloc((n+1)*sizeof(unsigned long int)))==NULL) {
   printf("Memory allocation error\n");
   exit(0);
 }
 if((model[m].freq = (unsigned long int *) malloc((n+1)*sizeof(unsigned long int)))==NULL) {
   printf("Memory allocation error\n");
   exit(0);
 }
 if((model[m].char_to_index = (int *) malloc((n+1)*sizeof(int)))==NULL) {
   printf("Memory allocation error\n");
   exit(0);
 }
 if((model[m].index_to_char = (int *) malloc((n+1)*sizeof(int)))==NULL) {
   printf("Memory allocation error\n");
   exit(0);
 }

 for (i=0; i< n; i++)
 { 			         /* Set up tables that  */
     model[m].char_to_index[i] = i+1;     /* translate between symbol */
     model[m].index_to_char[i+1] = i;     /* indexes and characters */ 
 }

 model[m].cum_freq[n] = 0;
 model[m].freq[0] = 0;
 for (i=n; i>0;i--) 
 {           		 		/* Set up initial frequency */
     model[m].freq[i] = 1;        		/* counts to be one for all */
     model[m].cum_freq[i-1] = model[m].cum_freq[i] + model[m].freq[i] ;    	/* symbols     */
 }

 //model[m].freq[0]=0;          /* freq[0] must not be the */
    	                     /* same as freq[1]   */

}

void ArithmeticDecoder :: restart_model(int m)
{ 

 int i;
 
 int n=model[m].no_of_char;


 for (i=0; i< n; i++)
 { 			         /* Set up tables that  */
     model[m].char_to_index[i] = i+1;     /* translate between symbol */
     model[m].index_to_char[i+1] = i;     /* indexes and characters */ 
 }

 model[m].cum_freq[n] = 0;
 model[m].freq[0] = 0;
 for (i=n; i>0;i--) 
 {           		 		/* Set up initial frequency */
     model[m].freq[i] = 1;        		/* counts to be one for all */
     model[m].cum_freq[i-1] = model[m].cum_freq[i] + model[m].freq[i] ;    	/* symbols     */
 }

 //model[m].freq[0]=0;          /* freq[0] must not be the */
    	                     /* same as freq[1]   */

}

/*  Includes a new char in model m   */
void ArithmeticDecoder :: add_new_char(int m)
{

   int i;
   unsigned long int cum;					

   i = model[m].no_of_char;
 //Memory allocation
 if((model[m].cum_freq = (unsigned long int *) realloc(model[m].cum_freq, (i+2)*sizeof(unsigned long int)))==NULL) {
   printf("Memory allocation error\n");
   exit(0);
 }
 if((model[m].freq = (unsigned long int *) realloc(model[m].freq, (i+2)*sizeof(unsigned long int)))==NULL) {
   printf("Memory allocation error\n");
   exit(0);
 }
 if((model[m].char_to_index = (int *) realloc(model[m].char_to_index, (i+2)*sizeof(int)))==NULL) {
   printf("Memory allocation error\n");
   exit(0);
 }
 if((model[m].index_to_char = (int *) realloc(model[m].index_to_char, (i+2)*sizeof(int)))==NULL) {
   printf("Memory allocation error\n");
   exit(0);
 }

 if (model[m].cum_freq[0]==Max_freq) 		/* See if frequency counts	*/
	{					/* are at their maximum		*/					
     cum = 0;
     for (i = model[m].no_of_char; i>=0; i--) 	/* If so, halve all the 	*/
		{				/* counts ( keeping them	*/
         model[m].freq[i] = (model[m].freq[i]+1)/2;		/* non-zero).			*/
         model[m].cum_freq[i] = cum;						
         cum += model[m].freq[i];
        }
   }
   model[m].freq[0] = 0;

  i = model[m].no_of_char;

   model[m].char_to_index[i]  = i+1;
   model[m].index_to_char[i+1]= i;
   model[m].freq[i+1] = 1;
   model[m].cum_freq[i]  = 1;
   model[m].cum_freq[i+1]= 0;
   while(i)
   {
      i--;
      model[m].cum_freq[i]++;
   }
   (model[m].no_of_char)++;   

 
}

/* UPDATE THE MODEL TO ACCOUNT FOR A NEW SYMBOL */
void ArithmeticDecoder :: update_model(int symbol, int m)
{ 

 int i;                  /* New index for symbol  */
 unsigned long int cum;					

 symbol = model[m].char_to_index[symbol];

 if (model[m].cum_freq[0]==Max_freq) 		/* See if frequency counts	*/
	{					/* are at their maximum		*/					
     cum = 0;
     for (i = model[m].no_of_char; i>=0; i--) 	/* If so, halve all the 	*/
		{				/* counts ( keeping them	*/
         model[m].freq[i] = (model[m].freq[i]+1)/2;		/* non-zero).			*/
         model[m].cum_freq[i] = cum;						
         cum += model[m].freq[i];
        }
    }
    model[m].freq[0] = 0;

 /* Find symbol's new index */
 for (i = symbol; model[m].freq[i]==model[m].freq[i-1]; i--) ;
 if (i<symbol) 
 {
     int ch_i, ch_symbol;

     ch_i = model[m].index_to_char[i];     /* Update the translation  */
     ch_symbol = model[m].index_to_char[symbol];   /* tables if the symbol has */
     model[m].index_to_char[i] = ch_symbol;    /* moved     */
     model[m].index_to_char[symbol] = ch_i;
     model[m].char_to_index[ch_i] = symbol;
     model[m].char_to_index[ch_symbol] = i;
  }

  model[m].freq[i] += 1;
  while (i>0)
  {
    i--;
    model[m].cum_freq[i]++;
  }

}

void ArithmeticDecoder :: add_model(void) {

  Nmodels++;
  if(model == NULL) {
    if((model = (PModel *) malloc(Nmodels*sizeof(PModel))) == NULL) {
      printf("Memory allocation error\n");
      exit(0);
    }
  }
  else {
    if((model = (PModel *) realloc(model, Nmodels*sizeof(PModel))) == NULL) {
      printf("Memory allocation error\n");
      exit(0);
    }
  }
}

void ArithmeticDecoder :: copy_model_mask(int m, int *mask) {

 int n, k;

 n = model[m].no_of_char;
 if(temp_model.cum_freq != NULL) free(temp_model.cum_freq);
 if(temp_model.freq != NULL) free(temp_model.freq);
 if(temp_model.char_to_index != NULL) free(temp_model.char_to_index);
 if(temp_model.index_to_char != NULL) free(temp_model.index_to_char);
 //Memory allocation
 if((temp_model.cum_freq = (unsigned long int *) malloc((n+1)*sizeof(unsigned long int)))==NULL) {
   printf("Memory allocation error\n");
   exit(0);
 }
 if((temp_model.freq = (unsigned long int *) malloc((n+1)*sizeof(unsigned long int)))==NULL) {
   printf("Memory allocation error\n");
   exit(0);
 }
 if((temp_model.char_to_index = (int *) malloc((n+1)*sizeof(int)))==NULL) {
   printf("Memory allocation error\n");
   exit(0);
 }
 if((temp_model.index_to_char = (int *) malloc((n+1)*sizeof(int)))==NULL) {
   printf("Memory allocation error\n");
   exit(0);
 }

 k = 1;
 for(n = 0; n < model[m].no_of_char; n++) {
  if(mask[n] == 1) {
    temp_model.char_to_index[n] = k;
    temp_model.index_to_char[k] = n;
    k++;
  }
 } 
 temp_model.no_of_char = k-1;
 for(n = 0; n < temp_model.no_of_char; n++) {
  //temp_model.freq[n+1] = model[m].freq[model[m].char_to_index[n]];
  temp_model.freq[n+1] = model[m].freq[model[m].char_to_index[temp_model.index_to_char[n+1]]];
 }
 temp_model.freq[0] = 0; 
 temp_model.cum_freq[temp_model.no_of_char] = 0; 
 for(n = temp_model.no_of_char; n > 0; n--) {
  temp_model.cum_freq[n-1] = temp_model.cum_freq[n] + temp_model.freq[n];
 }
}
