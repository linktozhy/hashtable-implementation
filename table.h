
#include <string>
#include <utility>
#include <functional>
#include "mylist.h"

using namespace std;

template <class TYPE>
class Table {
public:
	Table() {}
	virtual void update(const string& key, const TYPE& value)=0;
	virtual bool remove(const string& key) = 0;
	virtual bool find(const string& key, TYPE& value) = 0;
	virtual int numRecords() const = 0;
	virtual bool isEmpty() const = 0;
	virtual ~Table() {}
};

template <class TYPE>
class SimpleTable :public Table<TYPE> {

	struct Record {
		TYPE data_;
		string key_;
		Record(const string& key, const TYPE& data) {
			key_ = key;
			data_ = data;
		}

	};

	Record** records_;   //the table
	int capacity_;       //capacity of the array

	void grow() {
		Record** tmp = new Record * [capacity_ + 10];
		for (int i = 0; i < capacity_; i++) {
			tmp[i] = records_[i];
		}
		for (int i = capacity_; i < capacity_ + 10; i++) {
			tmp[i] = nullptr;
		}
		delete[] records_;
		records_ = tmp;
		capacity_ += 10;
	}
public:
	SimpleTable(int capacity);
	SimpleTable(const SimpleTable& other);
	SimpleTable(SimpleTable&& other);
	virtual void update(const string& key, const TYPE& value);
	virtual bool remove(const string& key);
	virtual bool find(const string& key, TYPE& value);
	virtual const SimpleTable& operator=(const SimpleTable& other);
	virtual const SimpleTable& operator=(SimpleTable&& other);
	virtual ~SimpleTable();
	virtual bool isEmpty() const { return numRecords() == 0; }
	virtual int numRecords() const;
};

template <class TYPE>
int SimpleTable<TYPE>::numRecords() const {
	int rc = 0;
	for (int i = 0; records_[i] != nullptr; i++) {
		rc++;
	}
	return rc;
}



template <class TYPE>
SimpleTable<TYPE>::SimpleTable(int capacity) : Table<TYPE>() {
	records_ = new Record * [capacity];
	capacity_ = capacity;
	for (int i = 0; i < capacity_; i++) {
		records_[i] = nullptr;
	}
}

template <class TYPE>
SimpleTable<TYPE>::SimpleTable(const SimpleTable<TYPE>& other) {
	records_ = new Record * [other.capacity_];
	capacity_ = other.capacity_;
	for (int i = 0; i < other.numRecords(); i++) {
		update(other.records_[i]->key_, other.records_[i]->data_);
	}
}
template <class TYPE>
SimpleTable<TYPE>::SimpleTable(SimpleTable<TYPE>&& other) {
	capacity_ = other.capacity_;
	records_ = other.records_;
	other.records_ = nullptr;
	other.capacity_ = 0;
}

template <class TYPE>
void SimpleTable<TYPE>::update(const string& key, const TYPE& value) {
	int idx = -1;
	int size = numRecords();
	for (int i = 0; i < size; i++) {
		if (records_[i]->key_ == key) {
			idx = i;
		}
	}
	if (idx == -1) {
		if (size == capacity_) {
			grow();
		}
		records_[size++] = new Record(key, value);
		for (int i = 0; i < size - 1; i++) {
			for (int j = 0; j < size - 1 - i; j++) {
				if (records_[j]->key_ > records_[j + 1]->key_) {
					Record* tmp = records_[j];
					records_[j] = records_[j + 1];
					records_[j + 1] = tmp;
				}
			}
		}
	}
	else {
		records_[idx]->data_ = value;
	}

}

template <class TYPE>
bool SimpleTable<TYPE>::remove(const string& key) {
	int idx = -1;
	for (int i = 0; i < numRecords(); i++) {
		if (records_[i]->key_ == key) {
			idx = i;
		}
	}
	if (idx != -1) {
		delete records_[idx];
		int size = numRecords();
		for (int i = idx; i < size - 1; i++) {
			records_[i] = records_[i + 1];
		}
		records_[size - 1] = nullptr;
		return true;
	}
	else {
		return false;
	}
}

template <class TYPE>
bool SimpleTable<TYPE>::find(const string& key, TYPE& value) {
	int idx = -1;
	for (int i = 0; i < numRecords(); i++) {
		if (records_[i]->key_ == key) {
			idx = i;
		}
	}
	if (idx == -1)
		return false;
	else {
		value = records_[idx]->data_;
		return true;
	}
}

template <class TYPE>
const SimpleTable<TYPE>& SimpleTable<TYPE>::operator=(const SimpleTable<TYPE>& other) {
	if (this != &other) {
		if (records_) {
			int sz = numRecords();
			for (int i = 0; i < sz; i++) {
				remove(records_[0]->key_);
			}
			delete[] records_;
		}
		records_ = new Record * [other.capacity_];
		capacity_ = other.capacity_;
		int size = other.numRecords();
		for (int i = 0; i < size; i++) {
			update(other.records_[i]->key_, other.records_[i]->data_);
		}

	}
	return *this;
}
template <class TYPE>
const SimpleTable<TYPE>& SimpleTable<TYPE>::operator=(SimpleTable<TYPE>&& other) {
	swap(records_, other.records_);
	swap(capacity_, other.capacity_);
	return *this;
}
template <class TYPE>
SimpleTable<TYPE>::~SimpleTable() {
	if (records_) {
		int sz = numRecords();
		for (int i = 0; i < sz; i++) {
			remove(records_[0]->key_);
		}
		delete[] records_;
	}
}

template <class TYPE>
class ChainingTable:public Table<TYPE>{

	struct Record
	{
		TYPE data_;
		string key_;

		//constructor
		Record()
		{
			data_ = TYPE {};
			key_ = "";
		}
		//constructor that receives 2 parameters
		Record(const string& key, const TYPE& data)
		{
			key_ = key;
			data_ = data;
		}
		//copy constructor
		Record(const Record& other)
		{
			key_ = other.key_;
			data_ = other.data_;
		}
		//copy assignment operator
		const Record& operator=(const Record& other)
		{
			if (this != &other)
			{
				key_ = other.key_;
				data_ = other.data_;
			}
			return *this;
		}

		bool operator <(const Record& b) const {
			bool rc = false;
			if ((this->key_).compare(b.key_) < 0) {
				rc = true;
			}
			return rc;
		}
	
		bool operator==(const Record& b) const {
			bool rc = false;
			if ((this->key_).compare(b.key_) == 0) {
				rc = true;
			}
			return rc;
		}

				
	};
			  
	SortedList<Record>** records_; //the table	
	int capacity_;           //capacity of the array
	int numRecord;          //number of record saved in the table
	double maxLoadFactor_;
	std::hash<std::string> hashFunction; //declare hash function

	void grow() {
		SortedList<Record>** tmp = new SortedList<Record> * [capacity_ * 2]; // initialize a temporary table container
		for (int i = 0; i < capacity_ * 2; i++) {
			tmp[i] = nullptr;
		}

		for (int i = 0; i < capacity_; i++) {
			if (records_[i] != nullptr) {
				for (typename SortedList<ChainingTable<TYPE>::Record>::iterator it = records_[i]->begin(); it != records_[i]->end(); it++)
				{
					int idx = hashFunction((*it).key_) % (capacity_ * 2);    //rehash to get new idx
					Record* temp = new Record((*it).key_, (*it).data_);   //put data into temporary container
					if (tmp[idx] != nullptr) {       
						tmp[idx]->insert(*temp);     
			
					}
					else
					{
						tmp[idx] = new SortedList<Record>();
						tmp[idx]->insert(*temp);

					}
				}
				delete records_[i]; //empty old table

			}
		}
		records_ = tmp;  
		capacity_ *= 2;
	}
	
public:
	
	ChainingTable(int maxExpected, double maxLoadFactor);
	ChainingTable(const ChainingTable& other);
	ChainingTable(ChainingTable&& other);
	virtual int capacity()const;
	virtual void update(const string& key, const TYPE& value);
	virtual bool remove(const string& key);
	virtual bool find(const string& key, TYPE& value);
	virtual const ChainingTable& operator=(const ChainingTable& other);
	virtual const ChainingTable& operator=(ChainingTable&& other);
	virtual ~ChainingTable();	
	virtual bool isEmpty()const;
	virtual int numRecords()const;	
};

//constructor receives two parameters
template <class TYPE>
ChainingTable<TYPE>::ChainingTable(int maxExpected, double maxLoadFactor): Table<TYPE>()
{
	records_ = new SortedList<Record>*[maxExpected];
    maxLoadFactor_ = maxLoadFactor;
	capacity_ = maxExpected;
	numRecord = 0;

	for (int i = 0; i < maxExpected; i++)
	{
		records_[i] = nullptr;
	}
		
}

//copy constructor
template <class TYPE>
ChainingTable<TYPE>::ChainingTable(const ChainingTable<TYPE>& other)
{
	if (other.numRecord != 0)
	{
		records_ = new SortedList<Record>*[other.capacity_];
		capacity_ = other.capacity_;
		numRecord = other.numRecord;

		for (int i = 0; i < other.capacity_; i++)
		{
			if (other.records_[i] == nullptr)
			{
				records_[i] = nullptr;
			}
			else
			{
				records_[i] = new SortedList<Record>();

				for (typename SortedList<ChainingTable<TYPE>::Record>::iterator it = other.records_[i]->begin(); it != other.records_[i]->end(); it++)
				{
					records_[i]->insert(*it); 
				}
			}
		}
	}
}

//move constructor
template <class TYPE>
ChainingTable<TYPE>::ChainingTable(ChainingTable<TYPE>&& other)
{
	if (other.numRecord != 0)
	{
		capacity_ = other.capacity_;
		numRecord = other.numRecord;
		maxLoadFactor_ = other.maxLoadFactor_;
		other.capacity_ = 0;
		other.numRecord = 0;
		other.maxLoadFactor_ = 0;
		
		records_ = other.records_;
		other.records_ = nullptr;
		
	}

}

template <class TYPE>
int ChainingTable<TYPE>::capacity()const
{
	return capacity_;
}

 
//update function 
template <class TYPE>
void ChainingTable<TYPE>::update(const string& key, const TYPE& value)
{	
	size_t hash = hashFunction(key);
	size_t idx = hash % capacity_; 

	Record* tmp = new Record(key, value); 
	
	if (records_[idx] != nullptr)
	{

		bool found = false; // check if the key is found in the Sorted list

		for (typename SortedList<ChainingTable<TYPE>::Record>::iterator it = records_[idx]->begin(); it != records_[idx]->end(); it++)
		{
			if (key == (*it).key_)
			{
				(*it).data_ = value;
				found = true;
			}
		}
	
		if (!found)
		{
			records_[idx]->insert(*tmp);
			numRecord++;

		}
	}
	else
	{
			records_[idx] = new SortedList<Record>();
			records_[idx]->insert(*tmp);
			numRecord++;	
	}

	delete tmp;
	if (double (double(numRecord) / double(capacity_)) > maxLoadFactor_) {
		grow();
	}
}


template <class TYPE>
bool ChainingTable<TYPE>::remove(const string& key)
{
	size_t hash = hashFunction(key);
	size_t idx = hash % capacity_;
	
	if (records_[idx] != nullptr)
	{
		for (typename SortedList<ChainingTable<TYPE>::Record>::iterator it = records_[idx]->begin(); it != records_[idx]->end(); it++)
		{
			if (key == (*it).key_)
			{
				records_[idx]->erase(it);
				numRecord--;
				return true;
			}
		}
	}
	
	return false;
}

template <class TYPE>
bool ChainingTable<TYPE>::find(const string& key, TYPE& value)
{
	size_t hash = hashFunction(key);
	size_t idx = hash % capacity_;
		
	if (records_[idx] != nullptr)
	{
		for (typename SortedList<ChainingTable<TYPE>::Record>::iterator it = records_[idx]->begin(); it != records_[idx]->end(); it++)
		{
			if (key == (*it).key_)
			{
				value = (*it).data_;
				return true;
			}
		}
	}	
		
	return false;	
}

template <class TYPE>
bool ChainingTable<TYPE>::isEmpty()const
{
	return numRecords() == 0; 
}

template <class TYPE>
int ChainingTable<TYPE>::numRecords() const
{

	return numRecord;
}


//assignment operator
template <class TYPE>
const ChainingTable<TYPE>& ChainingTable<TYPE>::operator=(const ChainingTable<TYPE>& other)
{
	if (this != &other && other.numRecord != 0)
	{
			for (int i = 0; i < capacity_; i++)
			{
				if (records_[i] != nullptr)
				{
					delete records_[i];
				}
			}
		}
	
	    records_ = new SortedList<Record> * [other.capacity_];

		capacity_ = other.capacity_;
		numRecord = other.numRecord;
		maxLoadFactor_ = other.maxLoadFactor_;
		
		for (int i = 0; i < other.capacity_; i++)
		{
			if (other.records_[i] != nullptr)
			{			
				records_[i] = new SortedList<Record>();
				for (typename SortedList<ChainingTable<TYPE>::Record>::iterator it = other.records_[i]->begin(); it != other.records_[i]->end(); it++)
				{
					records_[i]->insert(*it);
				}
			}
			else
			{
				records_[i] = nullptr;
			}
		}
	

	return *this;
}

//move assignment operator
template <class TYPE>
const ChainingTable<TYPE>& ChainingTable<TYPE>::operator=(ChainingTable<TYPE>&& other)
{
	if (this != &other && other.numRecord != 0)
	{
		swap(records_, other.records_);
		swap(numRecord, other.numRecord);
		swap(capacity_, other.capacity_);		
	}
	maxLoadFactor_ = other.maxLoadFactor_;
	return *this;
}


template <class TYPE>
ChainingTable<TYPE>::~ChainingTable()
{
	for (int i = 0; i < capacity_; i++)
	{
		if (records_[i] != nullptr)
		{
			delete records_[i];
			numRecord--;
		}		
	}
	delete [] records_;
}