// TODO Implement Start, end , insert pointer, read pointer

int start, end, insert_offset, read_pointer;
void init(){
  end = 0;
  insert_offset = 0;
  read_pointer = 0;
}
// end is the last element populated in the array
void* insert(int* data_headpointer, char* command);{
  data_headpointer[insert_offset++] = command;
  end = insert_offset;
  read_pointer = end + 1;
}
//read the command then point to older command
void* read_up(data_headpointer){
  if ( read_pointer = 0 ){
    print(data_headpointer[read_pointer]);
  }

  else if ( 0 < read_pointer <= end+1){
  print(data_headpointer[--read_pointer]);
  }
}

//read the command then point to newer command
void* read_down(data_headpointer){
  if (read_pointer+1 > end){
    //CLEAR LINE
  }

  else if (read_pointer+1 <= end){
    print(data_headpointer[++read_pointer])
  }
}

  
