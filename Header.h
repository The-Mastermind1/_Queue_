#pragma once 
#include<iostream>
#include<algorithm>
#include<type_traits>
#include<initializer_list>
#include<utility>
#include<functional>
#include<new>
#include"Macros.h"

_PANAGIOTIS_BEGIN
template<typename _Ty>
class queue final {
private:
	class queue_node final {
	private:
		class secret_class {};
		template<class ..._Valty>
		queue_node(secret_class, _Valty &&..._Val):data{std::forward<_Valty>(_Val)...},next{nullptr}
		{
		}
	public:
		_Ty data;
		queue_node* next;
		queue_node() :data{}, next{ nullptr }
		{

		}
		queue_node(const _Ty& item)noexcept(noexcept(data=item)) :next{nullptr}
		{
			
			data = item;
		}
		queue_node(_Ty&& item)noexcept(noexcept(data=std::move(item))) :next{ nullptr }
		{
			data = std::move(item);
		}
		template<class ..._Valty>
		static queue_node* craft(_Valty&&..._Val) {
			queue_node* ptr = new (std::nothrow)queue_node(secret_class{}, std::forward<_Valty>(_Val)...);
			return ptr;
		}
	};
	std::size_t count;
	queue_node* head;
	queue_node* tail;
	template<typename _Valty>
	bool push_node(_Valty &&_Val) {
		queue_node* ptr = new (std::nothrow)queue_node(std::forward<_Valty>(_Val));
		if (ptr != nullptr) {
			if (count != 0) {
				tail->next = ptr;
				tail = ptr;
			}
			else {
				head = tail = ptr;
				
			}
			count++;
			return true;
		}
		else {
			return false;
		}
	}
	void pop_node() {
		if (count != 0) {
			queue_node* ptr = head;
			head = head->next;
			count--;
			delete ptr;
			if (head==nullptr)tail = nullptr;
			return;
		}
		throw pop_from_an_empty_queue_{ "tried to pop from an empty queue" };
	}
	void swap_queue(queue<_Ty>& other) {
		std::swap(head, other.head);
		std::swap(count, other.count);
		std::swap(tail, other.tail);
	}
	void clear()noexcept {
		static_assert(std::is_destructible_v<_Ty>, "the type must be destructible");
		while (head != nullptr) {
			queue_node* ptr{ head };
			head = head->next;
			delete ptr;
		}
		head = tail = nullptr;
		count = 0;
	}
	template<typename..._Valty>
	bool emplace_node(_Valty&&..._Val) {
		queue_node* ptr = queue_node::craft(std::forward<_Valty>(_Val)...);
		if (ptr != nullptr) {
			if (count != 0) {
				tail->next = ptr;
				tail = ptr;
			}
			else {
				head = tail = ptr;

			}
			count++;
			return true;
		}
		else {
			return false;
		}
	}
public:
	static_assert(std::is_object_v<_Ty>, "The C++ Standard forbids container adaptors of non-object types "
		"because of [container.requirements].");
	static_assert(!std::is_reference_v<_Ty>, "no references allowed");
	static_assert(!std::is_const_v<_Ty>, "no const types are allowed");
	static_assert(!std::is_volatile_v<_Ty>, "no volatile types are allowed");
	static_assert(!std::is_array_v<_Ty>, "no raw c arrays are allowed");
	queue()noexcept :head{ nullptr }, tail{ nullptr }, count{}
	{

	}
	queue(const std::initializer_list<_Ty>& l) :head{ nullptr }, tail{ nullptr }, count{ 0 }
	{
		const _Ty* b = l.begin();
		for (std::size_t i = 0; i < l.size(); i++) {
			if (push(*b)) {
				b++;
			}
			else {
				this->~queue();
				break;
			}

		}
	}
	queue(const queue<_Ty>& other):head{nullptr},tail{nullptr},count{}
	{
		
		queue_node* ptr{ other.head };
		while (ptr != nullptr) {
			if (push(ptr->data)) {
				ptr = ptr->next;
			}
			else {
				clear();
				break;
			}
		}
		
	}
	queue(queue<_Ty>&& other)noexcept :head{ nullptr }, tail{ nullptr }, count{} {
		std::swap(head, other.head);
		std::swap(tail, other.tail);
		std::swap(count, other.count);
	}
	bool push(const _Ty& data) {
		return push_node(data);
	}
	bool push(_Ty&& data) {
		return push_node(std::move(data));
	}
	
	bool empty()const noexcept {
		return count == 0;
	}
	std::size_t size()const noexcept {
		return count;
	}
	void pop() {
		pop_node();
	}
	_NODISCARD _Ty back()&& {
		if (count == 0) {
			throw tried_to_access_an_empty_queue_{ "tried to access from an empty queue " };
		}
		return std::move(tail->data);

	}
	_NODISCARD const _Ty back()const&& {
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
		return std::move(head->data);

	}
	_NODISCARD _Ty front()const&& {
		if (count == 0) {
			throw tried_to_access_an_empty_queue_{ "tried to access from an empty queue " };
		}
		return std::move(head->data);

	}
	void swap(queue<_Ty>& other)noexcept {
		swap_queue(other);

	}
	~queue()noexcept {
		clear();
	}
	template<typename ..._Valty>
	bool emplace(_Valty&&..._Val) {
		return emplace_node(std::forward<_Valty>(_Val)...);
	}
	queue<_Ty>& operator =(const queue<_Ty>& other)& {
		queue_node* prev1{ nullptr };
		queue_node* prev2{ nullptr };
		queue_node* curr1{ head };
		queue_node* curr2{ other.head };
		while (curr1 != nullptr && curr2 != nullptr) {
			curr1->data = curr2->data;
			prev1 = curr1;
			prev2 = curr2;
			curr1 = curr1->next;
			curr2 = curr2->next;
		}
		if (prev1 == nullptr && curr2 != nullptr || prev1 != nullptr && curr2 != nullptr) {
			//size<other.size
			while (curr2 != nullptr) {
				if (push(curr2->data)) {
					curr2 = curr2->next;
				}
				else {
					clear();
					break;
				}
			}
			return *this;
		}
		if (prev2 == nullptr && curr1 != nullptr) {
			clear();
			return *this;
			
		}
		if (curr1 != nullptr) {//size>other.size,throw extras
			tail = prev1;
			tail->next = nullptr;
			while (curr1 != nullptr) {
				prev1 = curr1;
				curr1 = curr1->next;
				delete prev1;
				count--;
			}

			return *this;
		}
		//if (curr2 != nullptr) {//size<other.size,allocate extra
		//	while (curr2 != nullptr) {
		//		if (push(curr2->data)) {
		//			curr2 = curr2->next;
		//		}
		//		else {
		//			clear();
		//			break;
		//		}
		//	}
		//	return *this;
		//}
		return *this;
	}
	queue<_Ty>& operator=(queue<_Ty>&& other) &noexcept{
		clear();
		std::swap(head, other.head);
		std::swap(count, other.count);
		std::swap(tail, other.tail);
		return *this;

	}
	void show() {
		queue_node* ptr{ head };
		while (ptr != nullptr) {
			std::cout << ptr->data << '\n';
			ptr = ptr->next;
		}
	}

};
_PANAGIOTIS_END
