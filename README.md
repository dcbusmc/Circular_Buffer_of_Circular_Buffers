# Circular_Buffer_of_Circular_Buffers
An interesting data structure which consists of an outer and inner circular buffer.

All .cpp and .h files created and compiled on Microsoft Visual Studio.

.txt files indicate recomended output for each test.  Every test has its own main() so only one may run with project at a time.

The basic implementation of this data structure is to create a "buffer of buffers". When the initial buffer is filled, a new buffer of 
doublethe size is created and values continuously added in.  Once the old buffer is empty, it can be removed.  Each buffer is circular
in nature, so values can wrap around and find open spaces.  The array holding the buffers is circular itself, so this structure has the
potential to grow quite large.

The provided tests simulate some of the grading requirements enforced for this project and test the limits of the structure.
