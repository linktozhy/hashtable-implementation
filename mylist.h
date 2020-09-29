template <typename T>

class SortedList
{
	struct Node
	{
		T data_;
		Node* next_;
		Node* prev_;

		Node(const T& data = {}, Node* nx = nullptr, Node* pr = nullptr)
		{
			data_ = data;
			next_ = nx;
			prev_ = pr;
		}
	};

	Node* front_;
	Node* back_;
	int size_;

public:
	class const_iterator
	{
		friend class SortedList; 
		Node* curr_;
		const SortedList* myList_;

		const_iterator(Node* curr, const SortedList* theList)
		{
			curr_ = curr;
			myList_ = theList;
		}

	public:
		//constructor
		const_iterator()
		{
			curr_ = nullptr;
			myList_ = nullptr;
		}

		const_iterator operator++()
		{
			curr_ = curr_->next_;
			return *this;
		}

		const_iterator operator++(int)
		{
			const_iterator old = *this;
			curr_ = curr_->next_;
			return old;
		}

		const_iterator operator--()
		{
			curr_ = curr_->prev_;

			return *this;
		}
	
		const_iterator operator--(int)
		{
			const_iterator old = *this;

			curr_ = curr_->prev_;

			return old;
		}

		bool operator==(const_iterator rhs) const
		{
			bool rc = false;

			if (curr_ == rhs.curr_)
			{
				rc = true;
			}

			return rc;
		}

		bool operator!=(const_iterator rhs) const
		{
			return !(*this == rhs);
		}

		const T& operator*()const { return curr_->data_; }
	};
	class iterator :public const_iterator
	{
		friend class SortedList;
 
		iterator(Node* curr, SortedList* theList) :const_iterator(curr, theList) {}
	public:
		//constructor
		iterator() :const_iterator() {}

		iterator operator++()
		{
			this->curr_ = this->curr_->next_;
			return *this;
		}

		iterator operator++(int)
		{
			iterator old = *this;
			this->curr_ = this->curr_->next_;
			return old;
		}

		iterator operator--()
		{
			this->curr_ = this->curr_->prev_;
			return *this;
		}

		iterator operator--(int)
		{
			iterator old = *this;
			this->curr_ = this->curr_->prev_;
			return old;
		}

		T& operator*() { return this->curr_->data_; }

		const T& operator*()const { return this->curr_->data_; }
	};
	SortedList();
	~SortedList();
	SortedList(const SortedList& rhs);
	SortedList& operator=(const SortedList& rhs);
	SortedList(SortedList&& rhs);
	SortedList& operator=(SortedList&& rhs);
	iterator begin() { return iterator(front_->next_, this); } //returns iterator to Node containing the first piece of data in the linked list
	iterator end() { return iterator(back_, this); }//returns iterator to the Node after the node containing the last piece of data of the linked list
	const_iterator cbegin() const { return const_iterator(front_->next_, this); } //returns const iterator to Node containing the first piece of data in the linked list
	const_iterator cend() const { return const_iterator(back_, this); }//returns const iterator to the Node after the node containing the last piece of data of the linked list
	iterator insert(const T& data);
	iterator search(const T& data);
	const_iterator search(const T& data) const;
	iterator erase(iterator it);
	iterator erase(iterator first, iterator last);
	bool empty() const;
	int size() const;
};


template <typename T>
SortedList<T>::SortedList()
{
	front_ = new Node();
	back_ = new Node();
	front_->next_ = back_;
	back_->prev_ = front_;
	size_ = 0;

}

template <typename T>
SortedList<T>::~SortedList()
{
	Node* curr = front_;
	while (curr)
	{
		Node* rm = curr;
		curr = curr->next_;
		delete rm;
	}

}

template <typename T>
SortedList<T>::SortedList(const SortedList& rhs)
{
	*this = rhs;

}

template <typename T>
SortedList<T>& SortedList<T>::operator=(const SortedList& rhs)
{
	if (this != &rhs && rhs.size_ != 0)
	{
		const_iterator it;

		if (size_ == 0)
		{
			front_ = new Node();
			back_ = new Node();
		}

		else
		{
			it = cbegin();


			while (it != cend())
			{
				Node* rm = it.curr_;
				it++;
				delete rm;

			}
			size_ = 0;
		}
	
		front_->next_ = back_;
		back_->prev_ = front_;

		for (it = rhs.cbegin(); it != rhs.cend(); it++)
		{
			insert(*it);
		}

	}

	return *this;

}

template <typename T>
SortedList<T>::SortedList(SortedList&& rhs)
{
	if (rhs.size_ != 0)
	{
		size_ = rhs.size_;
		rhs.size_ = 0;
		front_ = rhs.front_;
		back_ = rhs.back_;
		rhs.front_ = nullptr;
		rhs.back_ = nullptr;
	}
}

template <typename T>
SortedList<T>& SortedList<T>::operator=(SortedList&& rhs)
{

	if (this != &rhs && rhs.size_ != 0)
	{

		if (size_ == 0)
		{
			front_ = new Node();
			back_ = new Node();
		}

		swap(size_, rhs.size_);
		swap(front_, rhs.front_);
		swap(back_, rhs.back_);		
	}

	return *this;

}


template <typename T>
typename SortedList<T>::iterator SortedList<T>::insert(const T& data)
{
	iterator it = begin();
			

	while ((*it < data) && it != end())
	{
		it++;
	}
	
	Node* nn = new Node(data, it.curr_, it.curr_->prev_);
	it.curr_->prev_->next_ = nn;
	it.curr_->prev_ = nn;
	size_++;
	it.curr_ = nn;
			
	
	return it;
}


template <typename T>
typename SortedList<T>::iterator SortedList<T>::search(const T& data)
{
	for (iterator it = begin(); it != end(); it++)
	{
		if (data == *it)
		{
			return it;
		}
	}

	return end();
}


template <typename T>
typename SortedList<T>::const_iterator SortedList<T>::search(const T& data) const
{
	for (const_iterator it = cbegin(); it != cend(); it++)
	{
		if (data == *it)
		{
			return it;
		}
	}

	return cend();
}


template <typename T>
typename SortedList<T>::iterator SortedList<T>::erase(iterator it)
{

	if (it.curr_ != front_ && it.curr_ != back_)
	{
		Node* rm = it.curr_;
		rm->prev_->next_ = rm->next_;
		rm->next_->prev_ = rm->prev_;
		delete rm;
		it++;
		size_--;

	}

	return it;
}

template <typename T>
typename SortedList<T>::iterator SortedList<T>::erase(iterator first, iterator last)
{
	if (first.curr_ != front_)
	{
		first.curr_->prev_->next_ = last.curr_;
		last.curr_->prev_ = first.curr_->prev_;

		for (iterator it_ = first; it_ != last;)
		{
			Node* rm_ = it_.curr_;
			++it_;
			delete rm_;
			size_--;
		}
	}

	return last;

}


template <typename T>
bool SortedList<T>::empty() const
{
	return front_->next_ == back_;
}

template <typename T>
int SortedList<T>::size() const
{
	return size_;

}
