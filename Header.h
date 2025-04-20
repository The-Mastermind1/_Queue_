#pragma once
#include"Header1.h"
#include<iostream>
#include<algorithm>
#include<type_traits>
#include<new>
#include<cstdlib>
#include<utility>
#include<initializer_list>
_PANAGIOTIS_BEGIN
template<typename _Ty>
class Queue {
private:
	class Queue_Node {
	public:
		_Ty data;
		Queue_Node* next;
		template<typename T=_Ty>
		requires(std::is_nothrow_default_constructible_v<_Ty>)
		Queue_Node()noexcept :data{},next{nullptr}
		{

		}
		Queue_Node(const _Ty& item)noexcept(noexcept(data=item))
			:next{nullptr}
		{
			data = item;
		}
		Queue_Node( _Ty&& item) noexcept(noexcept(data =std::move( item)))
			:next{nullptr}
		{
			data =std::move(item);
		}
		
	};
	std::size_t count;
	Queue_Node* head;
	Queue_Node* tail;
public:
	static_assert(std::is_object_v<_Ty>, "The C++ Standard forbids container adaptors of non-object types "
		"because of [container.requirements].");
	static_assert(!std::is_reference_v<_Ty>, "no references allowed");
	static_assert(!std::is_const_v<_Ty>, "no const types are allowed");
	static_assert(!std::is_volatile_v<_Ty>, "no volatile types are allowed");
	Queue()noexcept :count{0},head{nullptr},tail{nullptr}
	{//contructor

	}
	Queue(const std::initializer_list<_Ty>& l) : head{ nullptr }, tail{nullptr}, count{ 0 }
	{
		const _Ty* b = l.begin();
		for (std::size_t i = 0; i < l.size(); i++) {
			if (push(*b)) {
				b++;
			}
			else {
				this->~Queue();
				break;
			}

		}
	}

	Queue(const Queue<_Ty>& other) :count{0}, head{nullptr},tail{nullptr}
	{
		if (this != &other) {
			if (other.count != 0) {//exei na mas dosei kati o allos 
				Queue_Node* ptr = other.head;
				head = new (std::nothrow)Queue_Node(ptr->data);
				if (head != nullptr) {
					tail = head;
					count++;
					
					for (std::size_t i = 1; i < other.count; i++) {
						tail->next = new (std::nothrow)Queue_Node(ptr->next->data);
						if (tail->next != nullptr) {
							ptr = ptr->next;
							tail = tail->next;
							count++;
						}
						else {
							this->~Queue();
							break;
						}
					}
				}
				
			}
		}
	}
	Queue (Queue<_Ty>&& other)noexcept :count{ 0 }, head{ nullptr }, tail{ nullptr }
	{
		if (this != &other) {
			head = other.head;
			tail = other.tail;
			count = other.count;
			other.head = other.tail = nullptr;
			other.count = 0;
		}
	}

	bool push(const _Ty&item) 
	{
		Queue_Node* ptr = new (std::nothrow)Queue_Node(item);
		if (count != 0) {
			
			if (ptr != nullptr) {
				tail->next = ptr;
				tail = tail->next;
				count++;
				return true;
			}


		}
		else {
			if (ptr != nullptr) {
				head = tail = ptr;
				count++;
				return true;
			}
		}
		return false;
	}
	bool push(_Ty&& item)
	{
		Queue_Node* ptr = new (std::nothrow)Queue_Node(std::move(item));
		if (count != 0) {

			if (ptr != nullptr) {
				tail->next = ptr;
				tail = tail->next;
				count++;
				return true;
			}


		}
		else {
			if (ptr != nullptr) {
				head = tail = ptr;
				count++;
				return true;
			}
		}
		return false;
	}
	void pop() 
	{
		if (count != 0) {
			Queue_Node* ptr = head;
			head = head->next;
			if (count == 1) {
				tail = nullptr;

			}
			delete ptr;
			count--;
		
		}
		else {
			throw pop_from_an_empty_queue_{ "tried to pop from an empty queue" };

		}
	}
	_NODISCARD _Ty back()&& {
		if (count == 0) {
			throw tried_to_access_an_empty_queue_{"tried to access from an empty queue "};
		}
		return std::move(tail->data);
		
	}
	_NODISCARD const _Ty back()const && {
		if (count == 0) {
			throw tried_to_access_an_empty_queue_{ "tried to access from an empty queue " };
		}
		return std::move(tail->data);

	}
	_NODISCARD const _Ty& back()const& {
		if (count == 0) {
			throw tried_to_access_an_empty_queue_{ "tried to access from an empty queue " };
		}
		return tail->data;

	}
	_NODISCARD _Ty& back()& {
		if (count == 0) {
			throw tried_to_access_an_empty_queue_{ "tried to access from an empty queue " };
		}
		return tail->data;

	}
	_NODISCARD const _Ty& front()const& {
		 if (count == 0) {
			 throw tried_to_access_an_empty_queue_{ "tried to access from an empty queue " };
		 }
		 return head->data;

	}
	_NODISCARD _Ty& front()& {
		 if (count == 0) {
			 throw tried_to_access_an_empty_queue_{ "tried to access from an empty queue " };
		 }
		 return head->data;

	}
	_NODISCARD _Ty front()&& {
		 if (count == 0) {
			 throw tried_to_access_an_empty_queue_{ "tried to access from an empty queue " };
		 }
		 return head->data;

	}
	_NODISCARD _Ty front()const&& {
		 if (count == 0) {
			 throw tried_to_access_an_empty_queue_{ "tried to access from an empty queue " };
		 }
		 return head->data;

	}

	_NODISCARD bool empty() const noexcept
	{
		return count == 0;
	}

	_NODISCARD std::size_t size() const noexcept
	{
		return count;
	}
	void swap(Queue<_Ty>& other)noexcept
	{
		std::swap(head, other.head);
		std::swap(tail, other.tail);
		std::swap(count, other.count);

	}

	Queue<_Ty>& operator =(const Queue<_Ty>& other)&
	{//copy constructor 
		if (this == &other)return *this;
		if (other.count != 0) {
			if (count == 0) {
				Queue_Node* ptr = other.head;
				head = new (std::nothrow)Queue_Node(ptr->data);
				if (head != nullptr) {
					tail = head;
					count++;

					for (std::size_t i = 1; i < other.count; i++) {
						tail->next = new (std::nothrow)Queue_Node(ptr->next->data);
						if (tail->next != nullptr) {
							ptr = ptr->next;
							tail = tail->next;
							count++;
						}
						else {
							this->~Queue();
							break;
						}
					}
				}
				return *this;
			}
			else {

				Queue_Node* ptr1 = head;
				Queue_Node* ptr2 = other.head;
				while (ptr1 != nullptr && ptr2 != nullptr) {

					while (ptr1->next != nullptr && ptr2->next != nullptr) {
						ptr1->data = ptr2->data;
						ptr1 = ptr1->next;
						ptr2 = ptr2->next;
					}
					ptr1->data = ptr2->data;
					break;

				}
				/*if (count == other.count) {
					Queue_Node* ptr1 = head;
					Queue_Node* ptr2 = other.head;
					while (ptr1 != nullptr && ptr2 != nullptr) {
						ptr1->data = ptr2->data;
						ptr1 = ptr1->next;
						ptr2 = ptr2->next;

					}
					return *this;
				}*/
				if (count < other.count) {
					/*Queue_Node* ptr1 = head;
					Queue_Node* ptr2 = other.head;
					while (ptr1 != nullptr && ptr2 != nullptr) {

						while (ptr1->next != nullptr && ptr2->next != nullptr) {
							ptr1->data = ptr2->data;
							ptr1 = ptr1->next;
							ptr2 = ptr2->next;
						}
						ptr1->data = ptr2->data;
						break;

					}*/
					ptr2 = ptr2->next;
					while (ptr2 != nullptr) {
						if (push(ptr2->data)) {
							ptr2 = ptr2->next;

						}
						else {
							this->~Queue();
							break;
						}
						

					}
					return *this;
				}
				else if (count > other.count) {
					/*Queue_Node* ptr1 = head;
					Queue_Node* ptr2 = other.head;
					while (ptr1 != nullptr && ptr2 != nullptr) {

						while (ptr1->next != nullptr && ptr2->next != nullptr) {
							ptr1->data = ptr2->data;
							ptr1 = ptr1->next;
							ptr2 = ptr2->next;
						}
						ptr1->data = ptr2->data;
						break;

					}*/
					tail = ptr1;
					ptr1 = ptr1->next;
					while (ptr1 != nullptr) {
						
						Queue_Node* ptr = ptr1;
						ptr1 = ptr1->next;
						delete ptr;
						count--;
					}
					return *this;

				}
				return *this;
			}

		}
		else {
			this->~Queue();
			return *this;
		}
		
	}

	Queue<_Ty>& operator =(Queue<_Ty>&& other)&noexcept
	{
		this->~Queue();
		if (other.count != 0) {
			head = other.head;
			tail = other.tail;
			count = other.count;
			other.head = other.tail = nullptr;
			other.count = 0;
		}
		return *this;
	}
	~Queue()noexcept 
	{
		for (std::size_t i = 0; i < count; i++) {
			std::cout << "hello\n";
			Queue_Node* ptr = head;
			head = head->next;
			delete ptr;
		}
		count = 0;
		head = tail = nullptr;
	}

};
_PANAGIOTIS_END
