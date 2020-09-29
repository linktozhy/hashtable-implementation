# hashtable-implementation

Part 3: Implementation of hash table with chaining for collision resolution 
Starter Files
table.h
mylist.h
Description
A hash table organizes the records as an array, where records are placed according to a hash value that you get from a hash function. Please use the hash function from the C++ STL. You can find more info about how to do it here:

http://en.cppreference.com/w/cpp/utility/hash

Here is a simple example of how to use the hash function with string keys:

#include <iostream>
#include <string>
//you need to include functional to use hash functions
#include <functional>
const int cap = 100;
int main(void){
    //this is just a string... any string can be a key
    std::string key = "this is my key";

    //declare an hash function object.  hashFunction is a variable but it is also a function
    std::hash<std::string> hashFunction;
 
    //you can now call hashFunction and it will return an unsigned whole number
    size_t hash = hashFunction(s);
    std::cout << hash << std::endl;
    
    //if you need to change it so that it is a proper index for a table with capacity of cap
    size_t idx = hash % cap;
    std::cout << idx << std::endl;
}
Aside from the hash function, you are NOT allowed to use anything else from the C++ Standard Library. However, you are allowed to use the linked lists you wrote for your labs 3/labs 4/assignment 1. Simply duplicate whichever list you wish to use into the file called mylist.h . The submitter will submit it as part of the submission process. I will want to see it as part of your a2 submission. You are also welcome add functionality to support what you need to the list that you use. However, you must place the list into the mylist.h file

For this derived class use the class name: ChainingTable

ChainingTable is a hash table that uses chaining as the collision resolution method. The chaining table has the following functionality:

ChainingTable(int maxExpected, double maxLoadFactor);
Your ChainingTable constructor should accept the following parameters:

maxExpected is the maximum number of records your table is expected to hold. This number indicates how many "slots" the table can have.
maxLoadFactor is the maximum load factor allowed before the table gets reconfigured to have twice as many slots. load factor is calculated as: (number of records stored)/(number of slots)
int capacity() const;
This function returns the number of slots in the table

bool update(const string& key, const TYPE& value);
This function is passed a key-value pair. If your table already has a record with a matching key, the record's value is replaced by the value passed to this function. If no record exists, a record with key-value pair is added.

If the update() causes the load factor to go above the maxLoadFactor for the table, this function must reallocate the number of slots in the table to be double the current number of slots before the new key-value pair gets passed in.

bool find(const string& key,TYPE& value );
This function is passed a key and a reference for passing back a found value. If your table contains a record with a matching key, the function returns true, and passes back the value from the record. If it does not find a record with a matching key, function returns false.

bool remove(const string& key);
This function is passed a key. If your table contains a record with a matching key, the record (both the key and the value) is removed from the table

bool isEmpty() const;
This function returns true if table is empty, false otherwise

int numRecords() const;
This function returns number of records in the table

Aside from the above functions, all tables must have proper destructors, copy constructors/assignment operators, move constructor/operator that allow for resources to be properly managed.
