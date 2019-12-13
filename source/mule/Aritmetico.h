/* DECLARATIONS USED FOR ARITHMETIC ENCODING AND DECODING */
#include<stdio.h>

#ifndef ARITMETICO_H
#define ARITMETICO_H

/* SIZE OF ARITHMETIC CODE VALUES */

#define Code_value_bits	32		/* Number of bits in a code value */
typedef unsigned long  int code_value;		/* Type of an arithmetic code value */

#define Top_value 	(((unsigned long long)1<<Code_value_bits)-1)	/* Largest code value */

/* HALF AND QUARTER POINTS IN THE CODE VALUE RANGE */

#define	First_qtr	((unsigned long int)Top_value/4+1)	/* Point after first quarter 	*/
#define Half		((unsigned long int)2*First_qtr)	/* Point after first half 		*/
#define Third_qtr 	((unsigned long int)3*First_qtr)	/* Point after third quarter 	*/

#define Max_freq 131071
//#define Max_freq 32768

typedef struct{
    int no_of_char;
    int *index_to_char;
    int *char_to_index;
    unsigned long int *freq;
    unsigned long int *cum_freq;
    float *log2freq;
    float log2cum;
}PModel;

class ArithmeticCoder {

  //Output file
  FILE *ofp;

  //CURRENT STATE OF THE ENCODING
  code_value low,high;  // ends of the current code-region 
  long bits_to_follow;  // Number of opposite bits to output after the
                               //next bit.   
  //int bit_stream; /* holds the coded bitstream */

  //int Bits_Count;  /* holds the bits count */

  //int Input_Bits_Count; /* holds the input bits count */


  /* THE BIT BUFFER */
  unsigned char buffer;	/* Bits buffered for output 		*/
  int bits_to_go;	/* Number of bits still in buffer	*/

  //PROBABILISTIC MODELS
  int Nmodels;
  PModel *model;
  PModel temp_model;

  void bit_plus_follow(int bit); 
  void output_bit(int bit);
  void done_outputing_bits(void);
 public:
  ArithmeticCoder(void);
  void InitCoder(void);                     //Initializes coder, deletes all models
  void add_model(void);                     // Creates a new probabilistic model
  void encode_symbol(int symbol, int m);    // Encode a symbol with model m
  void encode_symbol_tm(int symbol);        // Encode a symbol with temporary model 
  void done_encoding(void);                 // Flush bits out of the encoder
  void start_model(int n, int m);           // Initialize model m using n symbols
  void restart_model(int m);                // Initialize model m using n symbols
  void add_new_char(int m);                 // Includes a new symbol in model m
  void update_model(int symbol, int m);     // Update histogram tables for model m
  void set_output_file(char *nome);         // Opens the output file
  float rate(int symbol, int m);            // Evaluates the rate needed to encode symbol with model m
  float rate_tm(int symbol);                // Evaluates the rate needed to encode symbol with temp. model
  float entropy(int m);                     //Evaluates the entropy of model m
  void copy_model_mask(int m, int *mask);   //Copy unmasked elements of model m to temp
  void copy_model(int m, int n);            //Copy model n into model m
  void load_model(int m, PModel &source_model);   //initializes probabilistic model m with the model stored at source_model
  void get_model(int m, PModel &target_model);   //reads probabilistic model m  and store it at target_model
  void start_pmodel(int n, PModel &external_model);     //Initialize external_model using n symbols
  void delete_pmodel(PModel &external_model);           // Deletes external_model
  void print_model(int m);				// displays arithmetic model
};

class ArithmeticDecoder {
  FILE *ifp;

  /* CURRENT STATE OF THE DECODING */
  code_value value;		/* Currently-seen code value		*/
  code_value low,high;		/* Ends of current code region 		*/

/* The BIT BUFFER */

  unsigned char buffer;         /* Bits waiting to be input 		*/

  int bits_to_go;		/* Number of bits still in buffer	*/

  int garbage_bits;	        /* Number of bits past end-of-file 	*/

  //PROBABILISTIC MODELS
  int Nmodels;
  PModel *model;
  PModel temp_model;

  int input_bit(void);
 public:
  ArithmeticDecoder(void);  
  void add_model(void);
  int decode_symbol(int m);                 // Decode a symbol with model m
  int decode_symbol_tm(void);               // Decode a symbol with temporary model
  void start_model(int n, int m);           // Initialize model m using n symbols
  void restart_model(int m);         // Initialize model m using n symbols
  void add_new_char(int m);                 // Includes a new symbol in model m
  void update_model(int symbol, int m);     // Update histogram tables for model m
  void copy_model_mask(int m, int *mask);   //Copy unmasked elements of model m to temp
  void set_input_file(char *nome);
  void done_decoding(void);
};

#endif