#include <stdio.h>
#include "shift_shaper.h"

// FUNCTIONS--------------------------------------------------------------------


unsigned int get_bit(unsigned int reg, int tap){
  return ((reg>>tap) & 1);
}

unsigned int operate(unsigned int left, unsigned int right, char type){
  if (type == 'X'){
    return (left ^ right);
  }
  else if (type == 'A'){
    return (left & right);
  }
  else if (type == 'O'){
    return (left | right);
  }
  else{
    return 0;
  }
}

void shift_reg(unsigned int * reg, int number){
  *reg = *reg>>number;
}

void print_gate_array(struct Gate * array_gates, int length){
  for (int i=0; i<length; i++){
    printf("%i", i);
  }
}

void create_gate(struct Gate * gate, //array of gates
                 int * counter,
                 unsigned int * element1,
                 int tap1,
                 unsigned int * element2,
                 int tap2,
                 char type){

  if (*counter >= MAX_NUM_GATES){
    printf("Maximum number of gates has been reached!");
  }
  else{
    struct Gate new_gate;
    new_gate.element1 = element1;
    new_gate.tap1 = tap1;
    new_gate.element2 = element2;
    new_gate.tap2 = tap2;
    new_gate.type = type;
    new_gate.out = 0;
    new_gate.species = type;

    gate[*counter] = new_gate;
    *counter += 1;
    printf("Gate created.\n");
  }
}


struct Logic_Module create_logic_module(unsigned int * reg,
                                        unsigned int reg_tap){

  struct Logic_Module lm;
  lm.counter = 0;
  lm.reg = reg;
  lm.reg_inlet_value = reg_tap;
  lm.final_value = 0;

  return lm;
}

void compute_gate(struct Gate * a_gate){
  unsigned int x = get_bit(*a_gate->element1, a_gate->tap1);
  unsigned int y = get_bit(*a_gate->element2, a_gate->tap2);
  unsigned int z = operate(x, y, a_gate->type);
  a_gate->out =  z;
}

unsigned int compute_gate_array(struct Gate * a_gate, unsigned int array_length){
  unsigned int temp_output;
  for (int i=0; i<array_length; i++){
    compute_gate(&a_gate[i]);
    temp_output = a_gate[i].out;
  }
  return temp_output;
}

void compute_logic_module(struct Logic_Module * LM){
  LM->final_value = compute_gate_array(LM->array_of_gates, LM->counter);
  *LM->reg = *LM->reg | LM->final_value<<LM->reg_inlet_value;
}

void print_int_as_binary(unsigned int * reg){
  for(int i=31; i>-1; i--){
    unsigned int temp = *reg>>i;
    unsigned int temp2 = temp & 1;
    printf("%i", temp2);
  }
}

void print_reg_and_gates(struct Logic_Module * LM, unsigned int * reg){

  print_int_as_binary(reg);

  printf("      ");

  for(int i=0; i<LM->counter; i++){
    printf("%i", i);
  }

  printf("\n");

  for(int i=0; i<LM->counter; i++){
    for(int j=31; j>-1; j--){
      if (j == LM->array_of_gates[i].tap1){
        printf("|");
      }
      else if (j == LM->array_of_gates[i].tap2){
        printf("|");
      }
      else{
        printf(" ");
      }
    }

    printf("      ");

    for (int k=0; k<LM->counter; k++){
      if (k == i){
        printf("%c", LM->array_of_gates[i].type);
      }
      else{
        printf(" ");
      }
    }
    printf("\n");
  }
}
