/*Program that takes input of two words from the user and produces a ladder of
 * words between the two, taking into account all the words must be valid dictionary
 * word.*/

#include <iostream>
#include "console.h"
#include <fstream>
#include "filelib.h"
#include "simpio.h"
#include "hashset.h"
#include "queue.h"
#include "stack.h"
using namespace std;

void intro();
void readFile(HashSet<string>& dictionary);
bool emptyInput(string& endWord, string& startWord);
bool validWord(string& startWord, string& endWord, HashSet<string>& dictionary);
Queue<Stack<string> > findLadder(string& startWord, string& endWord, HashSet<string>& dictionary);
void createNewQueue(string word, Stack<string> stack, HashSet<string>& uniqueWords, Queue<Stack<string> >& currentQueue);
void displayResult(string& startWord, string& endword, Queue<Stack<string> >& finalQueue);

/*
 * Introduce the program to the user, read the input file into the dictionary and find the queue of stacks
 * using the starting and ending word. Then display the result.
 */
int main() {
    intro();
    HashSet<string> dictionary;
    readFile(dictionary);
    string endWord, startWord;
    while (! emptyInput(endWord, startWord)) {
        startWord = toLowerCase(startWord);
        endWord = toLowerCase(endWord);
        if (validWord(startWord, endWord, dictionary)) {
            Queue<Stack<string> > finalQueue = findLadder(startWord, endWord, dictionary);
            displayResult(startWord, endWord, finalQueue);
        }
    }
    cout << "Have a nice day." << endl;
    return 0;
}

// Introduce the program.
void intro() {
    cout << "Welcome to CS 106B/X Word Ladder!" << endl;
    cout << "Please give me two English words, and I will convert the" << endl;
    cout << "first into the second by modifying one letter at a time." << endl;
    cout << endl;
}

// Read file and put words into a hashset.
void readFile(HashSet<string>& dictionary) {
    ifstream input;
    string fileName = promptUserForFile("Dictionary file name: ");
    input.open(fileName);
    string line;
    while (getline(input, line)) {
        dictionary.add(line);
    }
    input.close();
    cout << endl;
}

// Check whether the user typed in an empty input. Exit immidiately if so.
bool emptyInput(string& endWord, string& startWord) {
    endWord = getLine("Word 1 (or Enter to quit): ");
    if (endWord.length() <= 0) {
        return true;
    }
    startWord = getLine("Word 2 (or Enter to quit): ");
    return (startWord.length() <= 0);
}

// Check whether the words are in the dictionary, have the same length and are different.
bool validWord(string& startWord, string& endWord, HashSet<string>& dictionary) {
    if (! dictionary.contains(startWord) || ! dictionary.contains(endWord)) {
        cout << "The two words must be found in the dictionary.";
    } else if (startWord.length() != endWord.length()) {
        cout << "The two words must be the same length.";
    } else if (startWord == endWord) {
        cout << "The two words must be different.";
    } else {
        return true;
    }
    cout << endl << endl;
    return false;
}

/*
 * Create a hash set that contains unique words. Then create the first queue that contains the starting
 * word only. Get the first stack from the queue, peek at the string and find neighbor words. If it is a
 * valid new words, put it into the unique words' hashset. Then make a new stack everytime it finds a
 * new unique words and put it back into the queue. Return the queue when it finds the ending word
 * (or when the queue becomes empty).
 */
Queue<Stack<string> > findLadder(string& startWord, string& endWord, HashSet<string>& dictionary) {
    HashSet<string> uniqueWords;
    Queue<Stack<string> > currentQueue;
    Stack<string> originalStack;
    createNewQueue(startWord, originalStack, uniqueWords, currentQueue);
    string newWord;
    while (newWord != endWord && currentQueue.size() != 0) {
        Stack<string> currentStack = currentQueue.dequeue();
        string word = currentStack.peek();
        for (int index = 0; index < word.length(); index++) {
            for (char j = 'a'; j <= 'z'; j++) {
                newWord = word;
                newWord.replace(index, 1, string("") + j);
                if (dictionary.contains(newWord) && newWord != startWord && ! uniqueWords.contains(newWord)) {
                    createNewQueue(newWord, currentStack, uniqueWords, currentQueue);
                }
                if (newWord == endWord) {
                    return currentQueue;
                }
            }
        }
    }
    return currentQueue;
}

// Enqueue everytime it finds a new unique valid word.
void createNewQueue(string word, Stack<string> stack, HashSet<string>& uniqueWords, Queue<Stack<string> >& currentQueue){
    uniqueWords.add(word);
    stack.push(word);
    currentQueue.enqueue(stack);
}

/*
 * Display the result. If the ladder exists, get the last stack from the queue. Then create a
 * new stack that invert the old stack. Finally, display the result one by one using the new
 * stack.
 */
void displayResult(string& startWord, string& endword, Queue<Stack<string> >& finalQueue) {
    if (finalQueue.size() == 0) {
        cout << "No word ladder found from " << startWord << " back to " << endword << ".";
    } else {
        Stack<string> invertedStack;
        while (finalQueue.size() != 0) {
            invertedStack = finalQueue.dequeue();
        }
        Stack<string> forDisplay;
        cout << "A ladder from " << startWord << " back to " << endword << ":" << endl;
        while (! invertedStack.isEmpty()) {
            forDisplay.push(invertedStack.pop());
        }
        while (! forDisplay.isEmpty()) {
            cout << forDisplay.pop() << " ";
        }
    }
    cout << endl << endl;
}
