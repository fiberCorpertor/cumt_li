//shared_ptr的简单实现版本
//基于引用记数的智能指针
//它可以和stl容器完美的配合
#pragma once

namespace kimi_boost
{

    template<class T>
    class shared_ptr
    {
    public:
        typedef T element_type;
        typedef T value_type;
        typedef T * pointer;
        typedef T& reference;
        typedef unsigned long size_type;

        explicit shared_ptr(T* p = 0) : px(p)
        {
            try { pn = new size_type(1); }
            catch (...) { delete(p); throw; }
        }
        shared_ptr& operator= (T* p)
        {
            if (this->px == p) return *this;
            dispose();
            try { pn = new size_type(1); }
            catch (...) { delete(p); throw; }
            px = p;
            return *this;
        }

        shared_ptr(const shared_ptr& r) throw() : px(r.px)
        {
            ++*r.pn;
            pn = r.pn;
        }
        shared_ptr& operator= (const shared_ptr& r) throw()
        {
            if (this == &r) return *this;
            dispose();
            px = r.px;
            ++*r.pn;
            pn = r.pn;
            return *this;
        }

        template<typename Y> friend class shared_ptr;
        //为了让有继续关系的shared_ptr类型赋值或构造
        template<typename Y>
        shared_ptr(const shared_ptr<Y>& r)
        {
            px = r.px;
            ++*r.pn;
            pn = r.pn; // shared_count::op= doesn't throw
        }
        template<typename Y>
        shared_ptr& operator= (const shared_ptr<Y>& r)
        {
            dispose();
            px = r.px;
            ++*r.pn;
            pn = r.pn; // shared_count::op= doesn't throw
            return *this;
        }

        template<typename Y>
        shared_ptr(Y* py)
        {
            try { pn = new size_type(1); }
            catch (...) { delete(py); throw; }
            px = py;
        }
        template<typename Y>
        shared_ptr& operator= (Y* py)
        {
            if (this->px == py) return *this;
            dispose();
            try { pn = new size_type(1); }
            catch (...) { delete(py); throw; }
            px = py;
            return *this;
        }

        ~shared_ptr() { dispose(); }

        void reset(T* p = 0)
        {
            if (px == p) return;
            if (--*pn == 0)
            {
                delete(px);
            }
            else
            { // allocate new reference
                // counter
                // fix: prevent leak if new throws
                try { pn = new size_type; }
                catch (...) {
                    // undo effect of —*pn above to
                    // meet effects guarantee
                    ++*pn;
                    delete(p);
                    throw;
                } // catch
            } // allocate new reference counter
            *pn = 1;
            px = p;
        } // reset


        reference operator*() const throw(){ return *px; }
        pointer operator->() const throw(){ return px; }
        pointer get() const throw(){ return px; }
        size_type use_count() const throw()//
        {
            return *pn;
        }
        bool unique() const throw()//
        {
            return *pn == 1;
        }

    private:
        void dispose() throw()
        {
            if (--*pn == 0)
            {
                delete px; delete pn;
            }
        }

        T * px;    // contained pointer
        size_type* pn;  // reference counter

    };  // shared_ptr


    template<typename A, typename B>
    inline bool operator==(shared_ptr<A> const & l, shared_ptr<B> const & r)
    {
        return l.get() == r.get();
    }

    template<typename A, typename B>
    inline bool operator!=(shared_ptr<A> const & l, shared_ptr<B> const & r)
    {
        return l.get() != r.get();
    }

}//namespace kimi_boost
