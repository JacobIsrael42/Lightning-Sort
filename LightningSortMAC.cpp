//**************************************************

// LightningSort

// A program that demonstrates sorting

// integer, double, and string arrays in linear time

// Author: Jacob Israel

// Contact: JacobIsrael@mail.USF.edu

// Last modified: 2014-Jan-13

//**************************************************

#include <limits> // INT_MAX UINT_MAX
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>
using namespace std;

const int CHECK_LENGTH = sizeof(double) / sizeof(unsigned int);
const int DOUBLE_SIZE = sizeof(double);

// __DARWIN_LITTLE_ENDIAN
#ifdef __LITTLE_ENDIAN__
bool LITTLE_ENDIAN_ORDER = true;
#else
bool LITTLE_ENDIAN_ORDER = false;
#endif

union doubleUnion{
	double doubleValue;
	unsigned int data[CHECK_LENGTH];
};

//--------------------------------------------------------------------------
// scrambleStringArray 
//--------------------------------------------------------------------------
void scrambleStringArray(string* stringArray, int length){
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
			tempString = tempString.substr(0, switchPos) + (char)(toupper(tempString[switchPos])) + tempString.substr(switchPos+1, tempString.length() - switchPos-1);
		}		
		stringArray[temp] = stringArray[i];
		stringArray[i] = tempString;
	}
}


//--------------------------------------------------------------------------
//                    shiftRight
// for doubles
//--------------------------------------------------------------------------
void shiftRight(doubleUnion &number){
	const static unsigned int topPosition = UINT_MAX - (UINT_MAX >> 1);	
	for(int i = 0 ; i < CHECK_LENGTH ; i++){
		if(! number.data[i]){
			continue;
		}
		number.data[i] >>= 1;
		if(number.data[i]){
			return;			
		}
		if(i < CHECK_LENGTH - 1){
			number.data[i+1] = topPosition;
		}
		return;
	}
} // end function


//--------------------------------------------------------------------------
//                notZero
// for doubleUnion
//--------------------------------------------------------------------------
bool notZero(doubleUnion number){
	for(int i = 0 ; i < CHECK_LENGTH ; i++){
		if(number.data[i]){
			return true;
		}
	}
	return false;
} // end function


//--------------------------------------------------------------------------
//             andOperator
// for doubleUnion
//--------------------------------------------------------------------------
bool andOperator(doubleUnion num1, doubleUnion num2){
	for(int i = 0 ; i < CHECK_LENGTH ; i++){
		if(num1.data[i] & num2.data[i]){
			return true;
		}
	}
	return false;
} // end function


//--------------------------------------------------------------------------
//            recursiveLightningSort
// for doubles
//--------------------------------------------------------------------------
void recursiveLightningSort(double* doubleArray, int length, doubleUnion position){
	int lowCount = 0;
	doubleUnion temp;
	doubleUnion* tempArray = new doubleUnion[length];
	for(int i = 0 ; i < length ; i++){
		temp.doubleValue = doubleArray[i];
		if(! andOperator(temp, position)){
			tempArray[lowCount].doubleValue = doubleArray[i];
			lowCount++;
		}
	}
	int ptr = lowCount;
	for(int i = 0 ; i < length ; i++){
		temp.doubleValue = doubleArray[i];
		if( andOperator(temp, position)){
			tempArray[ptr].doubleValue = doubleArray[i];
			ptr++;
		}
	}
	// Copy back
	for(int i = 0 ; i < length ; i++){
		doubleArray[i] = tempArray[i].doubleValue;
	}
	delete [] tempArray;
	
	shiftRight(position);
	if(notZero(position)){
		if(lowCount > 1){
			recursiveLightningSort(doubleArray, lowCount, position);
		}
		if(lowCount < length - 1){
			recursiveLightningSort(&doubleArray[lowCount], length - lowCount, position);
		}
	}
} // end function


//--------------------------------------------------------------------------
//                    lightningSort
// for doubles
//--------------------------------------------------------------------------
void lightningSort(double* doubleArray, int length){
	// Sign Exponent Fraction
	// Single Precision 1 8 23
	// Double Precision 1 16 52
	int negativeCount = 0;
	double* tempArray = new double[length];
	// Place negatives
	for(int i = 0 ; i < length ; i++){
		if(doubleArray[i] < 0){
			tempArray[negativeCount++] = doubleArray[i];
		}
	}
	int ptr = negativeCount;
	// Place non-negatives
	for(int i = 0 ; i < length ; i++){
		if(doubleArray[i] >= 0){
			tempArray[ptr++] = doubleArray[i];
		}
	}
	// Copy back
	for(int i = 0 ; i < length ; i++){
		doubleArray[i] = tempArray[i];
	}
	delete [] tempArray;
	// 64 bit => 2.0 32 bit => 2.0
	doubleUnion startingPosition;
	startingPosition.doubleValue = 2.0000;
	/* startingPosition.data[0] = (UINT_MAX - (UINT_MAX >> 1)) >> 1;
	 for(int i = 1 ; i < CHECK_LENGTH ; i++){
	 startingPosition.data[i] = 0;
	 }
	 */
	// Subsort negatives
	if(negativeCount > 1){
		recursiveLightningSort(doubleArray, negativeCount, startingPosition);
	}
	// Subsort positives
	if(negativeCount < length - 1){
		recursiveLightningSort(&doubleArray[negativeCount], length - negativeCount, startingPosition);
	}
	// Reverse the negatives
	if(negativeCount > 1){
	   int halfNegativeCount = negativeCount/2;
	   double tempDouble;
	   for(int i = 0 ; i < halfNegativeCount ; i++){
	   	tempDouble = doubleArray[i];
	   	doubleArray[i] = doubleArray[negativeCount-i-1];
	   	doubleArray[negativeCount-i-1] = tempDouble;
	   }
	}
} // end function


//--------------------------------------------------------------------------
//                recursiveLightningSort
// for integers
//--------------------------------------------------------------------------
void recursiveLightningSort(int* integerArray, int length, int position){
	int lowCount = 0;
	int* tempArray = new int[length];
	for(int i = 0 ; i < length ; i++){
		if(! (integerArray[i] & position) ){
			tempArray[lowCount] = integerArray[i];
			lowCount++;
		}
	}
	int ptr = lowCount;
	for(int i = 0 ; i < length ; i++){
		if(integerArray[i] & position){
			tempArray[ptr] = integerArray[i];
			ptr++;
		}
	}
	// Copy back
	for(int i = 0 ; i < length ; i++){
		integerArray[i] = tempArray[i];
	}
	delete [] tempArray;
	position >>= 1;
	if(position > 0){
		if(lowCount > 1){
			recursiveLightningSort( integerArray, lowCount, position);
		}
		if(lowCount < length - 1){
			recursiveLightningSort( &integerArray[lowCount], length - lowCount, position);
		}
	}
} // end function


//--------------------------------------------------------------------------
//                  lightningSort
// Sorts an array of integers
// 32 or 64 bit
//--------------------------------------------------------------------------
void lightningSort(int* integerArray, int length){
	int negativeCount = 0;
	int* tempArray = new int[length];
	// Place negatives
	for(int i = 0 ; i < length ; i++){
		if(integerArray[i] < 0){
			tempArray [negativeCount++] = integerArray[i];
		}
	}
	int ptr = negativeCount;
	// Place non-negatives
	for(int i = 0 ; i < length ; i++){
		if(integerArray[i] >= 0){
			tempArray [ptr++] = integerArray[i];
		}
	}
	// Copy back
	for(int i = 0 ; i < length ; i++){
		integerArray[i] = tempArray[i];
	}
	delete [] tempArray;
	int startingPosition = INT_MAX - (INT_MAX >> 1); // 2^30 or 2^62
	// Subsort negatives
	if(negativeCount > 1){
		recursiveLightningSort(integerArray, negativeCount, startingPosition);
	}
	if(negativeCount < length - 1){
		// Subsort positives
		recursiveLightningSort(&integerArray[negativeCount], length - negativeCount, startingPosition);
	}
} // end function


//--------------------------------------------------------------------------
//                    recursiveLightningSort
// for strings
//--------------------------------------------------------------------------
void recursiveLightningSort(string* stringArray, int length, int charPosition){	
	// cout << "<<<<<<<< Array start " << stringArray[0] << " length: " << length << " char pos " << charPosition << " >>>>>"<<endl; cout.flush();
	const static int TOTAL_CHARS = 64;
	int shortStringCount = 0;
	int charCount[TOTAL_CHARS];
	for(int i = 0 ; i < TOTAL_CHARS ; i++){
		charCount[i] = 0;
	}
	int* charValue = new int[length];
	int* charPtr = new int[TOTAL_CHARS];
	// First iteration through the string array for counts
	for(int i = 0 ; i < length ; i++){
		if(stringArray[i].length() <= charPosition){
			shortStringCount++ ;
		}
		else{
			charValue[i] = toupper(stringArray[i][charPosition]) - ' '; // space == 0
			// Turn illegal characters into a space char
			if(charValue[i] < 0 || charValue[i] >= TOTAL_CHARS){
				charValue[i] = 0;
			}
			charCount[ charValue[i] ]++ ;
		}
	}
	int shortStringPtr = shortStringCount - 1;
	charPtr[0] = charCount[0] + shortStringPtr;
	// Turn integers into placement pointers
	for(int i = 1 ; i < TOTAL_CHARS ; i++){
		charPtr[i] = charCount[i] + charPtr[i-1] ;
	}
	string* tempStringArray = new string[length];
	// Place strings in-place in the new array
	for(int i = 0 ; i < length ; i++){
		if(stringArray[i].length() <= charPosition){
			tempStringArray[ shortStringPtr ] = stringArray[i] ;
			shortStringPtr-- ;
		}
		else{
			tempStringArray[ charPtr[charValue[i]] ] = stringArray[i] ;
			charPtr[charValue[i]]-- ;
		}
	}
	// Copy back
	for(int i = 0 ; i < length ; i++){
		stringArray[i] = tempStringArray[i];
	}
	delete [] tempStringArray;
	delete [] charValue;
	// Recusively subsort the strings that are not short
	// Subarrays must have length 2 or more
	// Sort by the next character position
	for(int i = 1 ; i < TOTAL_CHARS ; i++){
		if(charCount[i] > 1){
			// cout << "i = " << i << " count = " << charCount[i] << " offset = " << charPtr[i] << endl; cout.flush();
			recursiveLightningSort(&stringArray[ charPtr[i] + 1 ] , charCount[i], charPosition+1);
		}
	}
	delete [] charPtr;
} // end function


//--------------------------------------------------------------------------
//                  lightningSort
//  for strings
//-------------------------------------------------------------------------
void lightningSort(string* stringArray, int length){
	int startPosition = 0;
	// Move all empty strings to the beginning
	for(int i = 0 ; i < length ; i++){
		if(stringArray[i].length() <1){
			// Swap
			stringArray[i] = stringArray[startPosition];
			stringArray[startPosition] = "";
			startPosition++ ;
		}
	}
	recursiveLightningSort(&stringArray[startPosition], length-startPosition, 0);	
} // end function


//********************************************************************
// main
//********************************************************************
int main(){	
	cout << "\n\n StringSorter\nA program that demonstrates sorting a string array in linear time\nAuthor: Jacob Israel Copyright 2014\n";
	// Read the word list from a file of newline-seperated strings
	int listLength = 1800000;
	string* wordList = new string[listLength];
	ifstream wordFile("list.txt");
	if(! wordFile){
		cout << "\nSorry. I could not find list.txt the file with a list of strings (one string per line).\n";
		return 0;
	}
	// Load the strings
	for(int i = 0 ; i < listLength ; i++){
		wordFile >> wordList[i];
		// end of list
		if( wordList[i].length() <1){
			listLength = i-1;
		}
	}
	wordFile.close();
	// Scramble the array to show that it can be randomized
	// (not necessary)
	cout << "Scrambling" << endl; cout.flush();
	scrambleStringArray(wordList, listLength);
	
	cout << "\n\n_______________SCRAMBLED STRING ARRAY________________" << endl; cout.flush();
	for(int i = 0 ; i < listLength ; i++){
		cout << wordList[i] << endl;
	}
	
	cout << "\nSorting" << endl << endl;
	clock_t endTime, startTime = clock();
	lightningSort(wordList, listLength);
	endTime = clock();
	
	cout << "\n\n_______________SORTED STRING ARRAY________________" << endl;
	cout << wordList[0] << endl;
	for(int i = 1 ; i < listLength ; i++){
		for(int j=0 ; j < wordList[i].length() ; j++){
			if(wordList[i-1].length() <= j || toupper(wordList[i][j]) > toupper(wordList[i-1][j])){
				break;
			}
			if(wordList[i][j] >= ' ' && wordList[i][j]<= '~' && wordList[i-1][j] >= ' ' && wordList[i-1][j] <= '~' && toupper(wordList[i][j]) < toupper(wordList[i-1][j])){
				cout << "ERROR at char " << j << " of " << wordList[i] << " : "<< wordList[i+1] << endl;
				
				return(1);
			}
		}
		cout << wordList[i] << endl;
	}
	
	cout << "\nSorted " << listLength << " words in " << (double)(endTime - startTime )/CLOCKS_PER_SEC << " seconds"<< endl;
	
	// Integers
	cout << "\n\n____________UNSORTED INTEGERS________________" << endl;
	int numberOfIntegers = 18;
	int integerArray[numberOfIntegers];
	for(int i = 1 ; i < numberOfIntegers ; i++){
		if( rand() % 2){
			integerArray[i] = rand() ;
		}
		else{
			integerArray[i] = - rand() ;
		}
		cout << integerArray[i] << ", ";
	}
	cout << endl;
	
	startTime = clock();
	lightningSort(integerArray,numberOfIntegers);
	endTime = clock();
	cout << "\n\n____________SORTED INTEGERS________________" << endl;
	cout << integerArray[0] << endl;
	for(int i = 1 ; i < numberOfIntegers ; i++){
		cout << integerArray[i] << endl;
		if(integerArray[i] < integerArray[i-1] ){
			cout << "ERROR at " << i << endl;
			return(1);
		}
	}
	cout << "\nSorted " << numberOfIntegers << " integers in " << (double)(endTime - startTime )/CLOCKS_PER_SEC << " seconds"<< endl;
		

	// Doubles
	cout << "\n\n____________UNSORTED DOUBLES______________" << endl;
	int numberOfDoubles = 18;
	double doubleArray[numberOfDoubles];
	for(int i = 0 ; i < numberOfDoubles ; i++){ 
	   doubleArray[i] = rand()% 1000 * .00001837;// * pow(10.0, rand()%200 - 100) ;
		if(rand()%2 && doubleArray[i]!= 0){
			doubleArray[i] = - doubleArray[i];
		}
		cout << doubleArray[i] << endl;
	}
	cout << endl;
	startTime = clock();
	lightningSort(doubleArray,numberOfDoubles);
	endTime = clock();
	cout << "\n\n____________SORTED DOUBLES________________" << endl;
	cout << doubleArray[0] << endl;
	for(int i = 1 ; i < numberOfDoubles ; i++){
		cout << doubleArray[i] << endl;
		if(doubleArray[i] < doubleArray[i-1] ){
	      cout << "ERROR at " << i << endl;
		 	return(1);
		}
	}	
	cout << "\nSorted " << numberOfDoubles << " doubles in " << (double)(endTime - startTime )/CLOCKS_PER_SEC << " seconds"<< endl;
	return 0;	
}

