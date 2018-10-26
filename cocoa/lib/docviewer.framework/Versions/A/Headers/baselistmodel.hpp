#ifndef MatrixModel_H
#define MatrixModel_H

#include <QtCore/QtCore>
#include "macros.hpp"


class Controller;
class BaseListModelSignals : public QObject {
    Q_OBJECT



    PROP_DEF_BEGINS
    SIG_TF_0(model_reset)
    SIG_TF_3(rows_moved, int, int, int)
    SIG_TF_2(rows_removed, int, int)
    SIG_TF_2(rows_inserted, int, int)
    PROP_DEF_ENDS

public:
        BaseListModelSignals() = default;
};

template<class T>
class BaseListModel
{

    friend class Controller;
protected:
    QList<T> m_list;
    QSemaphore m_semaphore{100};
    void lock_for_read() { m_semaphore.acquire(1); }
    void unlock_for_read() { m_semaphore.release(1); }
    void lock_for_read_write() { m_semaphore.acquire(100); }
    void unlock_for_read_write() { m_semaphore.release(100); }

public:
    BaseListModelSignals sig;

    BaseListModel() = default;

    int count()
    {
        lock_for_read();
        int n = m_list.count();
        unlock_for_read();
        return n;
    }

    T& get(int row)
    {
        lock_for_read();
        T& rt = m_list[row];
        unlock_for_read();
        return rt;
    }

    T const& get(int row) const
    {
        lock_for_read();
        T const& rt = m_list[row];
        unlock_for_read();
        return rt;
    }

protected:
    bool insertRows(int row, int n)
    {
        if (row >= 0 && row <= count())
        {
            lock_for_read_write();
            for (int i = row; i < n; i++) {
                m_list.insert(i, T());
            }
            unlock_for_read_write();
            sig.emit_tf_rows_inserted(row, n);
            return true;
        }
        return false;
    }

    bool removeRows(int row, int n)
    {
        if (row >= 0 && row < count()
                && row + n - 1 >= 0 && row + n - 1 <= count())
        {
            lock_for_read_write();
            for (int i = row; i < count(); i++) {
                m_list.removeAt(row);
            }
            unlock_for_read_write();
            sig.emit_tf_rows_removed(row, n);
            return true;
        }
        return false;
    }

    void resetModel(QList<T> const& model)
    {
        lock_for_read_write();
        m_list = model;
        unlock_for_read_write();
        sig.emit_tf_model_reset();
    }

    void clear()
    {
        lock_for_read_write();
        m_list.clear();
        unlock_for_read_write();
        sig.emit_tf_model_reset();
    }

    int indexOf(T const& target)
    {
        lock_for_read();
        int i = m_list.indexOf(target);
        unlock_for_read();
        return i;
    }


    void insert(T const& t, int i = 0)
    {
        lock_for_read_write();
        m_list.insert(i, t);
        unlock_for_read_write();
        sig.emit_tf_rows_inserted(i, 1);
    }

    void remove(int i = 0)
    {
        lock_for_read_write();
        m_list.removeAt(i);
        unlock_for_read_write();
        sig.emit_tf_rows_removed(i, 1);
    }

    void remove(T const& t)
    {
        int i;
        lock_for_read_write();
        while ((i = m_list.indexOf(t)) > -1) {
            m_list.removeAt(i);
            sig.emit_tf_rows_removed(i, 1);
        }
        unlock_for_read_write();
    }
};


#endif // MatrixModel_H
