  1 #include <iostream>                                                                                                                                                                    
  2 #include "substring.h"                                                                                                                                                                 
  3                                                                                                                                                                                        
  4 using namespace std;                                                                                                                                                                   
  5                                                                                                                                                                                        
  6 int main()                                                                                                                                                                             
  7 {                                                                                                                                                                                      
  8                                                                                                                                                                                        
  9   cout << substring_position("this", "this is a simple exercise") << endl;                                                                                                             
 10   cout << substring_position("is", "this is a simple exercise") << endl;                                                                                                               
 11   cout << substring_position("is a", "this is a simple exercise") << endl;                                                                                                             
 12   cout << substring_position("is an", "this is a simple exercise") << endl;                                                                                                            
 13   cout << substring_position("exercise", "this is a simple exercise") << endl;                                                                                                         
 14   cout << substring_position("simple exercise", "this is a simple") << endl;                                                                                                           
 15   cout << substring_position("", "this is a simple exercise") << endl;                                                                                                                 
 16   cout << substring_position("", "") << endl;                                                                                                                                          
 17                                                                                                                                                                                        
 18                                                                                                                                                                                        
 19   return 0;                                                                                                                                                                            
 20 }  
