#ifndef QDEBUGSTREAM_H
#define QDEBUGSTREAM_H

#include <iostream>
#include <streambuf>
#include <string>
#include <QPlainTextEdit>
#include "threadsafe_queue.h"

class MainServerWindow;

class TextEditWrapper : public QObject
{
    Q_OBJECT

    friend class MainServerWindow;
    friend class MainClientWindow;

public:

    void appendPlainText(const QString& text)
    {
        m_queue.push(text);
        emit outputRequest();
    }

signals:
    void outputRequest();

private:

    ThreadSafeQueue<QString> m_queue;
};

class WOutputStream : public std::basic_streambuf<wchar_t>
{
public:
    WOutputStream(std::wostream &stream, TextEditWrapper* text_edit)
        : m_stream(stream)
    {
        m_log_window = text_edit;
        m_old_buf = stream.rdbuf();
        stream.rdbuf(this);
    }
    ~WOutputStream()
    {
        // output anything that is left
        if (!m_string.empty())
            m_log_window->appendPlainText(
                QString::fromWCharArray(m_string.c_str()));

        m_stream.rdbuf(m_old_buf);
    }

protected:
    virtual int_type overflow(int_type v)
    {
        if (v == '\n')
        {
            m_log_window->appendPlainText(
                QString::fromWCharArray(m_string.c_str()));
            m_string.erase(m_string.begin(), m_string.end());
        }
        else
            m_string += v;

        return v;
    }

    virtual std::streamsize xsputn(const wchar_t *p, std::streamsize n)
    {
        m_string.append(p, p + n);

        std::size_t pos = 0;
        while (pos != std::wstring::npos)
        {
            pos = m_string.find('\n');
            if (pos != std::wstring::npos)
            {
                std::wstring tmp(
                    m_string.begin(),
                    (pos == 0) ?
                        m_string.begin() + pos :
                        m_string.begin() + pos - 1);

                m_log_window->appendPlainText(
                    QString::fromWCharArray(tmp.c_str()));
                m_string.erase(m_string.begin(), m_string.begin() + pos + 1);
            }
        }

        return n;
    }

private:
    std::wostream&   m_stream;
    std::wstreambuf* m_old_buf;
    std::wstring     m_string;


    TextEditWrapper* m_log_window;
};

#endif
