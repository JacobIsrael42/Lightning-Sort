//**********************************************************
// LightningSort
// A program that demonstrates sorting
// integer, double, and string arrays in linear time
// Author: Jacob Israel
// Contact: JacobIsrael@mail.USF.edu
// Last modified: 2014-Jan-20
// Fixes: Added code for Windows
// Changed into LightningSorter class
// Sorting by integer and double in-place
//**********************************************************
#include <limits> // INT_MAX UINT_MAX
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <fstream> 
#include <ctime>
#include <cmath>
#include <sstream>
#include <pthread.h>
#include <unistd.h>

#include "BankAccount.cpp"  // www.CorporateWebPros.com/BankAccount.cpp.txt

#if defined _WIN32 || defined _WIN64
#include <Windows.h>	
#endif

using namespace std;

const static int TOTAL_CHARACTERS = 256;
// const static uint8_t characterMapping[] = {' ', 'A', 'a', 'B', 'b', 'C', 'c', 'D', 'd', 'E', 'e', 'F', 'f', 'G', 'g', 'H', 'h', 'I', 'i', 'J', 'j', 'K', 'k', 'L', 'l', 'M', 'm', 'N', 'n', 'O', 'o', 'P', 'p', 'Q', 'q', 'R', 'r', 'S', 's', 'T', 't', 'U', 'u', 'V', 'v', 'W', 'w', 'X', 'x', 'Y', 'y', 'Z', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

static uint8_t characterValue[TOTAL_CHARACTERS];
const static uint8_t DEFAULT_CHARACTER_VALUE = 0;
pthread_once_t oneTimeMutex = PTHREAD_ONCE_INIT;


class LightningSorter{
	//_______________________________________________________________________________	
private:
	//_______________________________________________________________________________
	static const int CHECK_LENGTH = sizeof(double) / sizeof(unsigned long);
	static const int DOUBLE_SIZE = sizeof(double);	
	union DoubleUnion{
		double doubleValue;
		unsigned long data[CHECK_LENGTH];
	};	
	
	//==========================================================
	//                    shiftRight
	// for doubles
	// exclusively for a double with a single 1 bit
	// Not for public use
	//==========================================================
	static void shiftRight(double &number){
		static DoubleUnion dblUnion;
		dblUnion.doubleValue = number;
		const static unsigned int TOP_POSITION = UINT_MAX - (UINT_MAX >> 1);
		for(int i = 0 ; i < CHECK_LENGTH ; i++){
         if(dblUnion.data[i] == 0){
				// nothing to shift
            continue;
         }
			// Shift
			dblUnion.data[i] >>= 1;
			
         if(dblUnion.data[i]){
				// not shifted out
				number = dblUnion.doubleValue;
            return;
         }
			// 1 was shifted out
			if(i > 0){
				// shift into next lower set of bytes
            dblUnion.data[i-1] = TOP_POSITION;
			}
			// A 1 was shifted, so we're done
			number = dblUnion.doubleValue;
			return;
		}
	} // end function
	
	
	//==========================================================
	//             andOperator
	// used for DoubleUnion to check for any two bits that are the same
	// Not a regular AND operation
	// Not for public use
	//==========================================================
	static bool andOperator(const double &num1, const double &num2){
		static DoubleUnion number1;
		static DoubleUnion number2;
		number1.doubleValue = num1;
		number2.doubleValue = num2;
		for(int i = 0 ; i < CHECK_LENGTH ; i++){
			if(number1.data[i] & number2.data[i]){
				return true;
			}
		}
		return false;
	} // end function
	
	
	//==========================================================
	//                  recursiveLightningSort
	// for doubles
	//==========================================================
	static void recursiveLightningSort(double* doubleArray, int length, double position){
		int lowCount = 0;
		int ptr0, ptr1;
      double tempDouble;
		while(lowCount == 0){
         for(int i = 0 ; i < length ; i++){ 
            if(! andOperator(doubleArray[i], position)){ 
               lowCount++; 
            }
         }
			// If all numbers sort to the same bucket
			// shift the check position right by one
			if (lowCount==0 || lowCount==length) {
				lowCount = 0;
				shiftRight(position);
				// If there is no more shifting to do
				// then all numbers are the same. Stop sorting.
				if (position==0) {
					return;
				}
			}
		}
		ptr0 = 0;
		ptr1 = lowCount;
		while(ptr0 < lowCount){ 
			while(ptr0 < lowCount && ! andOperator(doubleArray[ptr0], position)){ 
				ptr0++ ;
			}
			while(ptr1 < length && andOperator(doubleArray[ptr1], position)){ 
				ptr1++ ;	
			}
			if(ptr0 < lowCount){
			   // swap 
				tempDouble = doubleArray[ptr0];
				doubleArray[ptr0] = doubleArray[ptr1];
				doubleArray[ptr1] = tempDouble;
				ptr0++ ;
				ptr1++ ;
			}			
		}
		shiftRight(position);
		// Terminal case: position==0
		if(position > 0){ // notZero(position)
			if(lowCount > 1){
		 		recursiveLightningSort(doubleArray, lowCount, position);
			}
			if(lowCount < length - 1){
		    	recursiveLightningSort(&doubleArray[lowCount], length - lowCount, position);
			}
		}
	} // end function
	
	
	//==========================================================
	//                recursiveLightningSort
	// for integers
	//==========================================================
	static void recursiveLightningSort(int* integerArray, int length, int position){
		int lowCount = 0; 
		while (lowCount==0) {
			// Count how many 'low' numbers we have
			for(int i = 0 ; i < length ; i++){
				if(! (integerArray[i] & position) ){ 
					lowCount++;
				}
			}
			// If all numbers sort to the same bucket
			// shift the check position right by one
			if (lowCount==0 || lowCount==length) {
				lowCount = 0;
				position >>= 1;
				// If there is no more shifting to do
				// then all numbers are the same. Stop sorting.
				if (position==0) {
					return;
				}
			}
		}
		int tempInteger;
		int ptr0 = 0;
		int ptr1 = lowCount;
		while(ptr0 < lowCount){
			while(ptr0 < lowCount && !(integerArray[ptr0] & position)){ 
				ptr0++ ;
			}
			while(ptr1 < length && (integerArray[ptr1] & position)){ 
				ptr1++ ;
			}
			if(ptr0 < lowCount){
			   // Swap
				tempInteger = integerArray[ptr0];
				integerArray[ptr0] = integerArray[ptr1];
				integerArray[ptr1] = tempInteger;
				ptr0++ ;
				ptr1++ ;
			}			
		}
		position >>= 1;
		if(position > 0){
			// Don't sort arrays with less than 2 members
			if(lowCount > 1){
				recursiveLightningSort( integerArray, lowCount, position);
			}
			if(lowCount < length - 1){
				recursiveLightningSort( &integerArray[lowCount], length - lowCount, position);
			}
		}
	} // end function
	 
    
    //==========================================================
    //  initializeCharacterValueArray(voi
    // This maps characters to values. You can make your own mapping if you like.
    //==========================================================
    static void initializeCharacterValueArray(void){
        static bool firstTime = true;
        
        if(firstTime == true){
            firstTime = false;
            
            for(int i = 0 ; i <  TOTAL_CHARACTERS ; i++){
                characterValue[i] = DEFAULT_CHARACTER_VALUE;   // Turn 'illegal' characters into a space char
            }
            
            uint8_t index = 0;
            // Space is first
            characterValue[' '] = index;
            index++ ;
            for(int i = 'A' ; i <= 'Z' ; i++){
                characterValue[i] = index;
                index++ ;
            }
            int lowerCaseDifference = 'A' - 'a';
            
            for(uint8_t i = 'a' ; i <= 'z' ; i++){
                characterValue[ i ] =   characterValue[(uint8_t) (i + lowerCaseDifference) ];
            }
            for(uint8_t i = '0' ; i <= '9' ; i++){
                characterValue[ i ] = index;
                index++ ;
            }
        }
    }
    
	//==========================================================
	//                    getCharacterValue
    // Get the sort order of a character
    // Lower values should be sorted before higher values
	//==========================================================
    static int getCharacterValue(char theCharacter){
#if defined __APPLE__ || defined __linux__
        pthread_once(&oneTimeMutex, initializeCharacterValueArray);
#elif defined _WIN32 || defined _WIN64
        initializeCharacterValueArray();
#endif
        return characterValue[theCharacter];
    }

    //==========================================================
    //                   recursiveStringSortByCase(s
    //  Sort strings of the same length and text, that possibly have different cases (Upper / Lower)
    // Should never be called on an array of strings that are different (except by case)
    //==========================================================
    static void recursiveStringSortByCase(string* stringArray, int arrayLength, int charPosition){
        if (charPosition >= stringArray[0].length() ) {
            return;
        }
        string swapString;
        int lowerCaseIndex = arrayLength - 1;
        int upperCaseIndex = 0;
        
        while (upperCaseIndex < lowerCaseIndex ) {
            while (upperCaseIndex < lowerCaseIndex  && toupper(stringArray[upperCaseIndex][charPosition]) == stringArray[upperCaseIndex][charPosition])  {
                upperCaseIndex++ ;
            }
            while (upperCaseIndex < lowerCaseIndex && tolower(stringArray[lowerCaseIndex][charPosition]) == stringArray[lowerCaseIndex][charPosition]) {
                lowerCaseIndex-- ;
            }
            // Swap
            if(upperCaseIndex < lowerCaseIndex) {
                swapString = stringArray[lowerCaseIndex];
                stringArray[lowerCaseIndex] = stringArray[upperCaseIndex];
                stringArray[upperCaseIndex] = swapString;
                upperCaseIndex++ ;
                lowerCaseIndex-- ;
            }
        }
        // At least 2 strings to sort in each bucket ?
        if(upperCaseIndex >= 2){
            recursiveStringSortByCase(stringArray, upperCaseIndex, charPosition + 1);
        }
        if(arrayLength - upperCaseIndex >= 2){
            recursiveStringSortByCase(&stringArray[upperCaseIndex], arrayLength - upperCaseIndex, charPosition + 1);
        }
    }

	//==========================================================
	//                    recursiveLightningSort
	//  for strings
	//==========================================================
	static void recursiveLightningSort(string* stringArray, int arrayLength, int charPosition){

		int shortStringCount = 0;
		int charCount[TOTAL_CHARACTERS];
		int charPtr[TOTAL_CHARACTERS];
		for(int i = 0 ; i < TOTAL_CHARACTERS ; i++){
			charCount[i] = 0;
		}
		int* charValue = new int[arrayLength];
        string* tempStringArray = new string[arrayLength];
        
        // First iteration through the string array for counts of each character
        for(int i = 0 ; i < arrayLength ; i++){
			if(stringArray[i].length() <= charPosition){
				shortStringCount++ ;
			}
			else{
				charValue[i] = getCharacterValue(stringArray[i][charPosition]);
                charCount[ charValue[i] ]++ ;
            }
        }
 
        int shortStringPtr = 0;
        // Each pointer points to the last position in the 'bucket'
        charPtr[0] = shortStringCount;
        // Turn integers into placement pointers
        for(int i = 1 ; i < TOTAL_CHARACTERS ; i++){
            charPtr[i] = charPtr[i-1] + charCount[i-1];
        }
        
        // Place strings in-place in the new array
        for(int i = 0 ; i < arrayLength ; i++){
            if(stringArray[i].length() <= charPosition){
                tempStringArray[ shortStringPtr ] = stringArray[i] ;
                shortStringPtr++ ;
            }
            else{
                tempStringArray[ charPtr[charValue[i]] ] = stringArray[i];
                charPtr[charValue[i]]++ ;
            }
        }
        
        // Move the  will end up at the front of their buckets
        charPtr[0] = shortStringCount;
        for(int i = 1 ; i < TOTAL_CHARACTERS ; i++){
            charPtr[i] = charPtr[i-1] + charCount[i-1];
        }
        
        // Is there more than one string with the same exact letters ?
        // Sort these by Upper / Lower case
        if (shortStringCount >= 2) {
            recursiveStringSortByCase(tempStringArray, shortStringCount, 0);
         }
            
        // Copy back into the original array
        for(int i = 0 ; i < arrayLength ; i ++){
            stringArray[i] = tempStringArray[i];
        }
        
        delete [] tempStringArray;
        delete [] charValue;
        
        // Recusively subsort the strings that are not short
        for(int i = 0 ; i < TOTAL_CHARACTERS ; i++){
            // Subarrays must have length 2 or more
            if(charCount[i] > 1){    //                                                  Sort by the next character position
                recursiveLightningSort(&stringArray[ charPtr[i] ], charCount[i], charPosition+1);
            }
        }
    } // end function	
    
    
    //==========================================================
	//                recursiveLightningSort
	//  for sorting  objects  sorted by  integer
	// Requires that the objects have a function that 
	// takes an object pointer and returns an integer
   // stableSort uses more memory, while non-stable
   // sort will sort in place
	//==========================================================
	template <class T> static void recursiveLightningSort(T* objectArray[], int length, int position, int (* getInt)(T*), bool stableSort){
		int lowCount = 0;
		while (lowCount==0) {
			// Find all of the 'low' numbers
			for(int i = 0 ; i < length ; i++){
				if(! (getInt(objectArray[i]) & position) ){ 
					lowCount++;
				}
			}
			// If all numbers sort to the same bucket
			// shift the check position right by one
			if (lowCount==0 || lowCount==length) {
				lowCount = 0;
				position >>= 1;
				// If there is no more shifting to do
				// then all numbers are the same. Stop sorting.
				if (position==0) {
					return;
				}
			}
		} 
      int ptr0 = 0;
      int ptr1 = lowCount;
      // Sort into a new array
      if(stableSort){ 
         T** tempArray = new T*[length];
         for(int i = 0 ; i < length ; i++){
				if(! (getInt(objectArray[i]) & position) ){
               tempArray[ptr0] = objectArray[i];
               ptr0++ ;
            }
            else{
               tempArray[ptr1] = objectArray[i];
               ptr1++ ;
            }
         }       
         // Copy back
         for(int i = 0 ; i < length ; i++){
            objectArray[i] = tempArray[i]; 
         }
         // Free memory
         delete [] tempArray;
      }
      // Sort in place (unstable: will not preserve the previous sort)
      else {
         T* tempObjectPtr;
         while (ptr0 < lowCount) {
            while (ptr0 < lowCount && !(getInt(objectArray[ptr0]) & position)) {
               ptr0++ ;
            }
            while (ptr1 < length && (getInt(objectArray[ptr1]) & position)) {
               ptr1++ ;
            }
            if (ptr0 < lowCount) {
               // Swap
               tempObjectPtr = objectArray[ptr0];
               objectArray[ptr0] = objectArray[ptr1];
               objectArray[ptr1] = tempObjectPtr;	
               ptr0++ ;
               ptr1++ ;
            }
         }
      }
      // Recursive call if there are still more bits to compare
		position >>= 1;
		if(position > 0){
			if(lowCount > 1){
				recursiveLightningSort(objectArray, lowCount, position, getInt, stableSort);
			}
			if(lowCount < length - 1){
				recursiveLightningSort( &objectArray[lowCount], length - lowCount, position, getInt, stableSort);
			}
		}
	} // end function 
	
	
	//==========================================================
	//                recursiveLightningSort
	//  for sorting  objects  sorted by  double
	// Requires that the objects have a function that 
	// takes an object pointer and returns a double
	//==========================================================
	template <class T> static void recursiveLightningSort(T* objectArray[], int length, double position, double (* getDbl)(T*), bool stableSort){
		int lowCount = 0;
		// temporary object pointer
	   T* tempObjectPtr;
		while (lowCount==0) { 
			// Count low numbers
			for(int i = 0 ; i < length ; i++){ 
				if(! andOperator( getDbl(objectArray[i]), position)){ 
					lowCount++;
				}
			}
			// If all numbers sort to the same bucket
			// shift the check position right by one
			if (lowCount==0 || lowCount==length) {
				lowCount = 0;
				shiftRight(position);
				// If there is no more shifting to do
				// then all numbers are the same. Stop sorting.
				if (position==0) {
					return;
				}
			}
		} 
      // -----This is the actual sort-----
      int ptr0 = 0;
      int ptr1 = lowCount;
      
      // Sort into a new array
      if(stableSort){         
         T** tempArray = new T*[length];
         for(int i = 0 ; i < length ; i++){
				if(! andOperator( getDbl(objectArray[i]), position)){
               tempArray[ptr0] = objectArray[i];
               ptr0++ ;
            }
            else{
               tempArray[ptr1] = objectArray[i];
               ptr1++ ;
            }
         }      
         // Copy back
         for(int i = 0 ; i < length ; i++){
            objectArray[i] = tempArray[i]; 
         }
         // Free memory
         delete [] tempArray;   
      }
      // Sort in place (not stable, but faster)
      else{
         // low objects right, high objects left
         while (ptr0 < lowCount){
            while (ptr0 < lowCount && (! andOperator( getDbl(objectArray[ptr0]), position)) ) {
               ptr0++ ;
            }
            while (ptr1 < length && ( andOperator( getDbl(objectArray[ptr1]), position))) {
               ptr1++ ;
            }
            if(ptr0 < lowCount){
               // Swap
               tempObjectPtr = objectArray[ptr0];
               objectArray[ptr0] = objectArray[ptr1];
               objectArray[ptr1] = tempObjectPtr;				 
               ptr0++ ;
               ptr1++ ;
            }
         }
      }
      // Recursive call if there are still more bits to compare
		shiftRight(position);
		if(position > 0){ 
			if(lowCount > 1){
		 	   recursiveLightningSort(objectArray, lowCount, position, getDbl, stableSort);
			}
			if(lowCount < length - 1){
	       	recursiveLightningSort( &objectArray[lowCount], length - lowCount, position, getDbl, stableSort);
			}
		}
	} // end function  
	
	
	//==========================================================
	//                    recursiveLightningSort
	//  for sorting  objects  sorted by  a string
	// Requires that the objects have a function that 
	// takes an object pointer and returns a string
	//==========================================================
	template <class T> static void recursiveLightningSort(T* objectArray[], int length, int charPosition, string (* getString)(T*)){
		T** tempObjectArray = new T*[length];
		int shortStringCount = 0;
		int charCount[TOTAL_CHARACTERS];
		int charPtr[TOTAL_CHARACTERS];		
		for(int i = 0 ; i < TOTAL_CHARACTERS ; i++){
			charCount[i] = 0;
		}
		int* charValue = new int[length];

		string tempString;
		// First iteration through the string array for counts
		for(int i = 0 ; i < length ; i++){
			tempString = getString(objectArray[i]);
			if(tempString.length() <= charPosition){
				shortStringCount++ ;
			}
			else{
				charValue[i] = getCharacterValue(tempString[charPosition]);
				charCount[ charValue[i] ]++ ;
			}
		}
		charPtr[0] = shortStringCount;// charCount[0] + shortStringPtr;
		// Turn integers into placement pointers
		for(int i = 1 ; i < TOTAL_CHARACTERS ; i++){
			charPtr[i] = charPtr[i-1] + charCount[i-1];
		}		
		// Place strings in-place in the new array
		int ptr = 0;
		for(int i = 0 ; i < length ; i++){
			if(getString(objectArray[i]).length() <= charPosition){
				tempObjectArray[ptr] = objectArray[i];
				ptr++ ;
			}
			else{ 
				tempObjectArray[ charPtr[charValue[i]] ] = objectArray[i];
				charPtr[charValue[i]]++ ;
			}
		}
		// Copy back
		memcpy(objectArray, tempObjectArray, sizeof(T*)*length);
		
		delete [] tempObjectArray;
		delete [] charValue;
		// Recursively subsort the strings that are not short
		// Subarrays must have length 2 or more
		// Sort by the next character position
		for(int i = 0 ; i < TOTAL_CHARACTERS ; i++){
			if(charCount[i] > 1){
            recursiveLightningSort(&objectArray[ charPtr[i] - charCount[i] ] , charCount[i], charPosition+1, getString);
			}
		} 
	} // end function	
	
	
	
//_____________________________________________________________________		
public:  
//_____________________________________________________________________
	
	
	//==========================================================
	//                    scrambleStringArray 
	//==========================================================
	static void scrambleStringArray(string* stringArray, int length){
		string tempString;
		int temp;
		srand (time(NULL));
		int switchPos;
		for(int i = 0 ; i < length ; i++){
			temp = rand() % length;
			if(stringArray[temp].length() < 1){
				continue;
			}
			tempString = stringArray[temp];
			switchPos = rand() % tempString .length() ;
			if(rand()%2){
				tempString = tempString.substr(0, switchPos)  + (char)(toupper(tempString[switchPos])) +  tempString.substr(switchPos+1, tempString.length() - switchPos-1);
			}		
			stringArray[temp] = stringArray[i];
			stringArray[i] = tempString;
		}
	}
	

	//==========================================================
	//                    lightningSort
	// for doubles
	//==========================================================
	static void lightningSort(double* doubleArray, int length){
		//                Sign Exponent Fraction
		// Single Precision 1    8         23
		// Double Precision 1    16        52
		int negativeCount = 0; 
		// Count negatives
		for(int i = 0 ; i < length ; i++){
			if(doubleArray[i] < 0){
				 negativeCount++ ;
			}
		}
		int ptrNeg = 0;
		int ptrPos = negativeCount;
		double tempDouble;
		// Place numbers
		while(ptrNeg < negativeCount){
			while(ptrNeg < negativeCount && doubleArray[ptrNeg] < 0){ 
				ptrNeg++ ; 
			}
			while(ptrPos < length && doubleArray[ptrPos] >= 0){ 
				ptrPos++ ;
			}
			if (ptrNeg < negativeCount) {
				//Swap
				tempDouble = doubleArray[ptrNeg]; 
				doubleArray[ptrNeg] = doubleArray[ptrPos];
				doubleArray[ptrPos] = tempDouble;
				ptrNeg++ ;
				ptrPos++ ;
			}
		}
		// 64 bit => 2.0 32 bit => 2.0
		const double STARTING_POSITION = 2.0000;
 
		// Subsort negatives
		if(negativeCount > 1){
		   recursiveLightningSort(doubleArray, negativeCount, STARTING_POSITION);
		}
		// Subsort positives
		if(negativeCount < length - 1){
			recursiveLightningSort(&doubleArray[negativeCount], length - negativeCount, STARTING_POSITION);
		}
		// Reverse the negatives
	 	if(negativeCount > 1){
			int halfNegativeCount = negativeCount/2;
			for(int i = 0 ; i < halfNegativeCount ; i++){
				tempDouble = doubleArray[i]; 
				doubleArray[i] = doubleArray[negativeCount-i-1];
				doubleArray[negativeCount-i-1] = tempDouble;
			}
		} 
	} // end function
	
	
	//==========================================================
	//                  lightningSort
	// Sorts an array of integers
	// 32 or 64 bit
	//==========================================================
	static void lightningSort(int* integerArray, int length){
		int negativeCount = 0;
		// Count negatives
		for(int i = 0 ; i < length ; i++){
			if(integerArray[i] < 0){
				negativeCount++ ;
			}
		}
		int tempInteger;
		int ptrNeg = 0;
		int ptrPos = negativeCount;
		// Place negatives left and positives right
		while(ptrNeg < negativeCount){
			 while(ptrNeg < negativeCount && integerArray[ptrNeg] < 0){
				 ptrNeg++ ;
			 }
			 while(ptrPos < length && integerArray[ptrPos] >= 0){
				ptrPos++ ;
			}
			if(ptrNeg < negativeCount){
			   //Swap
				tempInteger = integerArray[ptrNeg];
				integerArray[ptrNeg] = integerArray[ptrPos];
				integerArray[ptrPos] = tempInteger;
				ptrNeg++ ;
				ptrPos++ ;
			}
		} 
		const int STARTING_POSITION = INT_MAX - (INT_MAX >> 1); // 2^30 or 2^62
		// Subsort negatives
		if(negativeCount > 1){
			recursiveLightningSort(integerArray, negativeCount, STARTING_POSITION);
		}
		if(negativeCount < length - 1){
			// Subsort positives
			recursiveLightningSort(&integerArray[negativeCount], length - negativeCount, STARTING_POSITION);
		}
	} // end function
	
	
	//==========================================================
	//                  lightningSort
	//  for strings
	//=========================================================
	static void lightningSort(string* stringArray, int length){
		int startPosition = 0;
		// Move all empty strings to the beginning
		for(int i = 0 ; i < length ; i++){
			if(stringArray[i].length() < 1){
				// Swap
				stringArray[i] = stringArray[startPosition];
				stringArray[startPosition] = "";
				startPosition++ ;
			}
		}
        
        recursiveLightningSort(&stringArray[startPosition], length - startPosition, 0);
	} // end function 
	
	
	//==========================================================
	//                  lightningSort
	//  for sorting  objects  sorted by  integer
	// Requires that the objects have a function that 
	// takes an object pointer and returns an integer
	//========================================================= 
	template <class T> static void lightningSort(T* objectArray[], int length, int (*getInt)(T*), bool stableSort=false){
		T* tempObjectPtr;
		int negativeCount = 0;
		
		// Count negatives 
		for(int i = 0 ; i < length ; i++){ 
		   if(getInt(objectArray[i]) < 0){  
			   negativeCount++ ;
			} 
		} 
		int ptrNeg = 0;
		int ptrPos = negativeCount;
		
		while (ptrNeg < negativeCount) {
			while (ptrNeg < negativeCount && getInt(objectArray[ptrNeg]) < 0) {
				ptrNeg++ ;
			}
			while (ptrPos < length && getInt(objectArray[ptrPos]) >= 0) {
				ptrPos++ ;
			}
			if (ptrNeg < negativeCount) {
				// Swap
				tempObjectPtr = objectArray[ptrNeg];
				objectArray[ptrNeg] = objectArray[ptrPos];
				objectArray[ptrPos] = tempObjectPtr;
				ptrNeg++ ;
				ptrPos++ ;
			}
		}
		int STARTING_POSITION = INT_MAX - (INT_MAX >> 1); // 2^30 or 2^62
		// Subsort negatives
		if(negativeCount > 1){
         recursiveLightningSort<T>(objectArray, negativeCount, STARTING_POSITION, getInt, stableSort);
		}
		if(negativeCount < length - 1){
			// Subsort positives
			recursiveLightningSort<T>(&objectArray[negativeCount], length - negativeCount, STARTING_POSITION, getInt, stableSort);
		}
	} // end function
	
	
	//==========================================================
	//                    lightningSort
	//  for sorting  objects  sorted by double
	// Requires that the objects have a function that 
	// takes an object pointer and returns a double
	//========================================================= 
	template <class T> static void lightningSort(T* objectArray[], int length, double (*getDbl)(T*), bool stableSort=false){
		T* tempObjectPtr; // temp object pointer 
		int negativeCount = 0;
		// Find negatives
		for(int i = 0 ; i < length ; i++){ 
		   if(getDbl(objectArray[i]) < 0){
			   negativeCount++ ;
			} 
		} 
		int ptrNeg = 0;
		int ptrPos = negativeCount;
		while (ptrNeg < negativeCount) {
			while (ptrNeg < negativeCount && getDbl(objectArray[ptrNeg]) < 0){
				ptrNeg++ ;
			}
			while(ptrPos < length && getDbl(objectArray[ptrPos]) >= 0) {
				ptrPos++ ;
			}
			if(ptrNeg < negativeCount){
			   // swap
				tempObjectPtr = objectArray[ptrNeg];
				objectArray[ptrNeg] = objectArray[ptrPos];
				objectArray[ptrPos] = tempObjectPtr;
				ptrNeg++ ;
				ptrPos++ ;
			}
		}  
		// 64 bit => 2.0     32 bit => 2.0
		double STARTING_POSITION = 2.0000;
		// Subsort negatives
		if(negativeCount > 1){
	    	recursiveLightningSort(objectArray, negativeCount, STARTING_POSITION, getDbl, stableSort);
		}
		// Subsort positives
		if(negativeCount < length - 1){
		 	recursiveLightningSort(&objectArray[negativeCount], length - negativeCount, STARTING_POSITION, getDbl, stableSort);
		}
		// Reverse the negatives
		if(negativeCount > 1){
			int halfNegativeCount = negativeCount/2;
			for(int i = 0 ; i < halfNegativeCount ; i++){
				tempObjectPtr = objectArray[i];
				objectArray[i] = objectArray[negativeCount-i-1];
				objectArray[negativeCount-i-1] = tempObjectPtr;
			}
		}			
	} // end function
	
	
	//==========================================================
	//                  lightningSort
	//  for sorting  objects  sorted by strings
	// Requires that the objects have a function that 
	// takes an object pointer and returns a string
	//========================================================= 
	template <class T> static void lightningSort(T* objectArray[], int length, string (*getString)(T*)){
		int startPosition = 0;
		T* tempObjectPtr;
		// Move all empty strings to the beginning
		for(int i = 0 ; i < length ; i++){
			if(getString(objectArray[i]).length() < 1){
				// Swap
				tempObjectPtr = objectArray[i];
				objectArray[i] = objectArray[startPosition];
				objectArray[startPosition] = tempObjectPtr;
				startPosition++ ;
			}
		}
		recursiveLightningSort(&objectArray[startPosition], length-startPosition, 0, getString);	
	} // end function
	
}; // end class

//==========================================================
//                      getTime
//========================================================= 
string getTime(clock_t startTime, clock_t endTime){	
	double netTime = (endTime - startTime)/(double)CLOCKS_PER_SEC;
	stringstream ss;//(stringstream::in | stringstream::out);
	ss.precision(8);
	if(netTime < 0.001){
		ss << netTime*1000000 << " microSeconds"; 
	}
	else if(netTime < 1){
		ss << netTime*1000 << " milliSeconds"; 
	}
	else{
		ss << netTime << " seconds"; 
	}
	return ss.str();
}
	
//==========================================================
//                        getYesNo
//========================================================= 
bool getYesNo(string question){
   string userChoice="";
   cout << question << " (y/n): ";
   cin >> userChoice; 
   if (userChoice.length() >0 && userChoice[0]=='y') {
      return true;
   }
	return false;
}

//********************************************************************
//                                 main
//********************************************************************
int main(int argumentC, char** argumentV){
	
	string userInput;
   bool userAnswer;
	cout << "\n\n________Lightning Sorter________\nA program that demonstrates sorting a\nstring array, integer array, or double array\nin linear time\n\nAuthor: Jacob Israel\n";
	// Read the word list from a file of newline-seperated strings
	int listLength = 180;
	string* wordList = new string[listLength];

	char* fileLocation;
	if(argumentC > 1){ 
		fileLocation = argumentV[1];	
	}
	else{
		cout << "\n_________Usage_________\nPlease call this program with" << endl;
		cout << "the name of a file that contains"<< endl;
		cout << "a list of strings (one string per line)."<< endl << endl;
		cout << "Eaxmple:\n  LightningSort list.txt"<< endl << endl;
		return(0);
	}
	
	ifstream wordFile(fileLocation);
	if(! wordFile.good()){
		cout << "\nSorry. I could not find the text file with a list of strings (one string per line).\n";
#if defined __APPLE__ || defined __linux__
		sleep(3);
#elif defined _WIN32 || defined _WIN64
		Sleep(3);	
#endif
		return 0;
	}
	// Load the strings
	for(int i = 0 ; i < listLength ; i++){
		wordFile >> wordList[i];
		// end of list
		if( wordList[i].length() < 1){
			listLength = i;
		}
	}
    wordFile.close();
    
    if(getYesNo("Shall I scramble the string array ? ")){
        // Scramble the array to show that it can be randomized
        // (not necessary)
        LightningSorter::scrambleStringArray(wordList, listLength);
        
        cout << "\n\n_______________SCRAMBLED STRING ARRAY________________" << endl; cout.flush();
        for(int i = 0 ; i < listLength ; i++){
            cout << wordList[i] << endl;
        }
        cout << "\nArray scrambled" << endl;
    }
    cout << "\nSorting strings" << endl;
    clock_t endTime, startTime = clock();
    LightningSorter::lightningSort(wordList, listLength);
    endTime = clock();
    cout << "\nSorted " << listLength << " words in " << getTime(startTime, endTime) << endl;
    
    if(getYesNo("Shall I print the sorted string array ? ")){
        cout << "\n\n_______________SORTED STRING ARRAY________________" << endl;
        cout << wordList[0] << endl;
    }
    for(int i = 1 ; i < listLength ; i++){
        // for(int j = 0 ; j < wordList[i].length() ; j++){
            /*
             if(wordList[i-1].length() <= j || toupper(wordList[i][j]) > toupper(wordList[i-1][j])){
             break;
             }
             
             if(wordList[i][j] >= ' ' && wordList[i][j]<= '~' && wordList[i-1][j] >= ' ' && wordList[i-1][j] <= '~' && toupper(wordList[i][j]) < toupper(wordList[i-1][j])){
             cout << "ERROR at char " << j << " of " << wordList[i] << " : "<< wordList[i+1] << endl;
             #if defined __APPLE__ || defined __linux__
             sleep(3);
             #elif defined _WIN32 || defined _WIN64
             Sleep(3);
             #endif
             return(0);
             }
             */
            cout << wordList[i] << endl;
       // }
    }
    
    int numberOfIntegers;
    cout << "\n\nHow many integers would you like to sort ? ";
    cin >> numberOfIntegers;
    // Integers
    userAnswer = getYesNo("Shall I print the integers ? ");
    if(userAnswer)
      cout << "\n\n____________UNSORTED INTEGERS________________" << endl;
   
   int* integerArray = new int[numberOfIntegers];
   srand (time(NULL));
   for(int i = 0 ; i < numberOfIntegers ; i++){
      if( rand() % 2){
         integerArray[i] = rand() ;
      }
      else{
         integerArray[i] = - rand() ;
      }
      if(userAnswer)
         cout << integerArray[i] << endl;
   }
   cout << endl;
   
   startTime = clock();
   LightningSorter::lightningSort(integerArray, numberOfIntegers);
	endTime = clock();
   
   if(userAnswer){
	   cout << "\n\n____________SORTED INTEGERS________________" << endl;
      cout << integerArray[0] << endl;
   }
	for(int i = 1 ; i < numberOfIntegers ; i++){
      if(userAnswer)
         cout << integerArray[i] << endl;
		if(integerArray[i] < integerArray[i-1] ){
			cout << "ERROR at " << i << endl;
#if defined __APPLE__ || defined __linux__
			sleep(3);
#elif defined _WIN32 || defined _WIN64
			Sleep(3);	
#endif
			return(0);
		}
      
	} 
	cout << "\nSorted " << numberOfIntegers << " integers in " << getTime(startTime, endTime) << endl; 	
	
	// Doubles
	int numberOfDoubles;
	cout << "\n\nHow many doubles would you like to sort ? ";
	cin >> numberOfDoubles;
   userAnswer = getYesNo("Shall I print the doubles ? ");
      
   if(userAnswer)   
	   cout << "\n\n____________UNSORTED DOUBLES______________" << endl; 
	double* doubleArray = new double[numberOfDoubles];
	srand (time(NULL));
   for(int i = 0 ; i < numberOfDoubles ; i++){ 
      doubleArray[i] = rand() % 10  * pow(10.0, rand()%200 - 100) ;
      if(rand()%2 && doubleArray[i]!= 0){
         doubleArray[i] = - doubleArray[i];
      }
      if(userAnswer) 
         cout << doubleArray[i] << endl;
   }
   cout << endl;
   startTime = clock();
   LightningSorter::lightningSort(doubleArray,numberOfDoubles);
   endTime = clock();
   if(userAnswer){
      cout << "\n\n____________SORTED DOUBLES________________" << endl;
      cout << doubleArray[0] << endl; 
   }
   for(int i = 1 ; i < numberOfDoubles ; i++){
      if(userAnswer) 
         cout << doubleArray[i] << endl;
      if(doubleArray[i] < doubleArray[i-1] ){
         cout << "ERROR at " << i << endl;
#if defined __APPLE__ || defined __linux__
         sleep(3);
#elif defined _WIN32 || defined _WIN64
         Sleep(3);	
#endif
		return(0);
		}
	}
	cout << "\nSorted " << numberOfDoubles << " doubles in " << getTime(startTime, endTime) << endl; 	
	
	// Objects
   int numberOfObjects;
	cout << "\n\nHow many Bank Accounts would you like to sort ? ";
	cin >> numberOfObjects;
	cout << "\n___________UNSORTED OBJECTS_______________" << endl;	
	string firstNames[] = {"John", "Jane","Janee", "Ryan", "Ned", "Ted","Teddy", "Ed", "Edward", "Fred","Frederick", "Fredrick", "Frank","Franklin", "Hank", "Anne", "Annie", "Liz","Liza", "Lisa", "Jenny", "Jennifer"};
	string lastNames[] = {"Johnson", "Johns", "Jamison", "McGregor", "Jones", "Grant", "Grand", "Hollister", "Frederick", "Frederickson", "Hardy", "Hardee", "Arnold", "Lane", "Laney", "Larson", "Jamison", "James", "Harding", "Polk", "Andrews", "Hollis"};
	BankAccount* bankAccount[numberOfObjects]; 
	for(int i = 0 ; i < numberOfObjects ; i++){		
		bankAccount[i] = new BankAccount(rand()%9899+1000, (rand()%100000)/100.0, firstNames[rand()%13],lastNames[rand()%22]);
		// cout << "Created bank account: " << &(BankAccount::toString() );
		// int (BankAccount::* ptfptr) (void) = &(BankAccount::getAccountNumber);
		 cout <<  bankAccount[i]->toString() << endl; 
	}   
   //	 int (*functionPointer)(BankAccount*);
   // functionPointer = &BankAccount::getAccountNumber; 
	startTime = clock();
	LightningSorter::lightningSort<BankAccount>(bankAccount, numberOfObjects,&BankAccount::getAccountNumber); 
	endTime = clock();	
   cout <<  "\n_________OBJECTS SORTED BY INTEGER (account number) in " << getTime(startTime, endTime) << endl; 	
	for(int i = 0 ; i < numberOfObjects ; i++){		
		 cout <<  bankAccount[i]->toString()<< endl; 
	} 
   
   userAnswer = getYesNo("Do you want to preserve the sort for Account Number?");	
	startTime = clock(); 
	LightningSorter::lightningSort<BankAccount>(bankAccount, numberOfObjects,&BankAccount::getBalance, userAnswer); 
	endTime = clock();
	cout <<  "\n_________OBJECTS SORTED BY DOUBLE (balance) in " << getTime(startTime, endTime) << endl; 	
	for(int i = 0 ; i < numberOfObjects ; i++){		
		cout <<  bankAccount[i]->toString()<< endl; 
	}
   
   // Sort by sting is always a stable sort
	startTime = clock(); 
	LightningSorter::lightningSort<BankAccount>(bankAccount, numberOfObjects,&BankAccount::getFirstName); 
	endTime = clock();
	cout <<  "\n_________OBJECTS SORTED BY STRING (first name) in " << getTime(startTime, endTime) << endl; 	
	for(int i = 0 ; i < numberOfObjects ; i++){		
		cout <<  bankAccount[i]->toString()<< endl; 
	} 
    
   startTime = clock(); 
	LightningSorter::lightningSort<BankAccount>(bankAccount, numberOfObjects,&BankAccount::getLastName);
	endTime = clock();
	cout <<  "\n_________OBJECTS SORTED BY STRING (last name) in " << getTime(startTime, endTime) << endl; 	
	for(int i = 0 ; i < numberOfObjects ; i++){		
		cout <<  bankAccount[i]->toString()<< endl; 
	}  
	return 0;
}
