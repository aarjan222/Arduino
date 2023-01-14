/*
 * queue.h
 * 
 * Created : 11/16/2018
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#ifndef _QUEUE_CUSTOM_H_
#define _QUEUE_CUSTOM_H_

template <class T, size_t q_size>
class Queue
{
public:
        Queue():head_(0), tail_(0), curr_size_(0) { }
        Queue(Queue &&) = default;
        Queue(const Queue &) = default;
        Queue &operator=(Queue &&) = default;
        Queue &operator=(const Queue &) = default;
        ~Queue() {  }

        void insert(T elem) {
                ++curr_size_;
                queue_[head_++] = elem;
                head_ %= q_size;
        }

        T lookup() {
                // if( curr_size_ ) {
                --curr_size_;
                T elem = queue_[tail_++];
                tail_ %= q_size;
                return elem;
                // }
                // return 0;
        }

        uint32_t length() const { return curr_size_; }

        bool is_Empty() const { return length() == 0; }

private:
        uint32_t head_, tail_, curr_size_;
        T queue_[q_size];
};

#endif // !_QUEUE_CUSTOM_H_
