   #include <iostream>                                                                                                                                                                    
   #include "substring.h"                                                                                                                                                                 
  
   using namespace std;                                                                                                                                                                   
  
   //********recursive approach***************                                                                                                                                            
                                                                                                                                                                                          
   bool is_prefix(char *string1, char *string2) {                                                                                                                                         
                                                                                                                                                                                          
    if (!*string1) {                                                                                                                                                                     
      return true;                                                                                                                                                                       
    }                                                                                                                                                                                    
                                                                                                                                                                                         
    if (*string1 == *string2) {                                                                                                                                                          
        string1++;                                                                                                                                                                       
        string2++;                                                                                                                                                                       
        return is_prefix(string1, string2);                                                                                                                                              
      }                                                                                                                                                                                  
                                                                                                                                                                                         
    return false;                                                                                                                                                                        
   }                                                                                                                                                                                      
                                                                                                                                                                                         
  int substring_position(char *string1, char *string2) {                                                                                                                                 
                                                                                                                                                                                         
    if (is_prefix(string1, string2)) {                                                                                                                                                   
        return 0;                                                                                                                                                                          
    }                                                                                                                                                                                    
                                                                                                                                                                                         
    string2++;                                                                                                                                                                           
    if (!*string2) {                                                                                                                                                                     
        return -1;                                                                                                                                                                         
    }                                                                                                                                                                                    
                                                                                                                                                                                         
    int count = substring_position(string1, string2);                                                                                                                                    
    return (count < 0 ? count : count + 1);                                                                                                                                              
  }                                                                                                                                                                                      
                                                                                                                                                                                         
  //********iterative approach***************                                                                                                                                            
  /*                                                                                                                                                                                     
  int substring_position(char s1[MAX], char s2[MAX])                                                                                                                                     
  {                                                                                                                                                                                      
   int count1(0), count2(0), count3(0);                                                                                                                                                  
                                                                                                                                                                                         
   while ((s1[count1] != '\0') && (s2[count2] != '\0'))                                                                                                                                  
     {                                                                                                                                                                                   
       if (s1[count1] == s2[count2])                                                                                                                                                     
         {                                                                                                                                                                               
           count1++;                                                                                                                                                                     
           count2++;                                                                                                                                                                     
         }                                                                                                                                                                               
       else if (s1[count1] != s2[count2])                                                                                                                                                
         {                                                                                                                                                                               
           count1 = 0;
           count2 = count3;                                                                                                                                                              
           count3++;                                                                                                                                                                     
         }                                                                                                                                                                               
     }                                                                                                                                                                                   
                                                                                                                                                                                         
   if (s1[count1] == '\0')                                                                                                                                                               
     {                                                                                                                                                                                   
       return (count2 - count1);                                                                                                                                                         
     }                                                                                                                                                                                   
   return -1;                                                                                                                                                                            
  }                                                                                                                                                                                      
  */              
 
 
