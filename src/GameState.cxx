#include <GameState.h>
#include <QApplication>
#include <QRandomGenerator>
#include <QColor>

#include <fstream>
GameState::eProcessRowResult GameState::ProcessRow()
{
    GameState::eProcessRowResult result;

    QString& word = m_game_state_array[m_row];
    if(word == m_word_hidden)
    {
        result = GameState::eProcessRowResult::WORD_IS_ANSWER;
    }
    else if(m_set_of_words.find(word) != m_set_of_words.end())
    {
        result = GameState::eProcessRowResult::WORD_EXISTS;
    }
    else
    {
        result = GameState::eProcessRowResult::WORD_DO_NOT_EXISTS;
    }
    return result;
};

QString GameState::GetRow(int row)
{
    return m_game_state_array[row];
};

QVector<QColor> GameState::ProcessRowColors(const QString& qStr)
{
    QVector<QColor> result(COLS_NUM);
    for(qsizetype i=0; i<qsizetype(COLS_NUM); i++)
    {
        if(qStr.at(i) == m_word_hidden.at(i))
        {
            result[i] = Qt::green;
        }
        else if(m_word_hidden.contains(qStr.at(i)))
        {
            result[i] = Qt::yellow;
        }
        else
        {
            result[i] = Qt::lightGray;
        }
    }
    return result;
};

void GameState::InputChar(const QString& ch)
    {
        QString & qStr = m_game_state_array[m_row];

        if(ch == "<-")
        {
            if(qStr.length())
            {
                qStr.resize(qStr.length() - 1);
                emit signalUpdate(m_row);
            }
        }
        else if(ch == "Enter" && qStr.length() == COLS_NUM)
        {
            eProcessRowResult processRowResult = ProcessRow();
            if(processRowResult == eProcessRowResult::WORD_IS_ANSWER)
            {
                emit signalUpdateRowColors(m_row, ProcessRowColors(qStr));
                emit signalMsgBox(QString("Вы победили.\nВы отгадали слово:\n%1").arg(m_word_hidden));
                emit signalQuitOrRestart();
            }
            else if(processRowResult == eProcessRowResult::WORD_EXISTS)
            {
                emit signalUpdateRowColors(m_row, ProcessRowColors(qStr));
                if(m_row != ROWS_NUM-1)
                {
                    m_row++;
                }
                else
                {
                    emit signalMsgBox(QString("Вы проиграли.\nБыло загадано слово:\n%1").arg(m_word_hidden));
                    emit signalQuitOrRestart();
                }
            }
            else if(processRowResult == eProcessRowResult::WORD_DO_NOT_EXISTS)
            {
                emit signalMsgBox(QString("Такого слова не существует"));
            }
        }
        else if(ch == "Enter" && qStr.length() != COLS_NUM)
        {
            emit signalMsgBox(QString("Букв меньше чем надо"));
        }
        else
        {
            if(qStr.length() != COLS_NUM)
            {
                qStr.push_back(ch);
                emit signalUpdate(m_row);
            }
        }

    };

void GameState::AddWordsFromFile(std::filesystem::path p)
{
    namespace fs = std::filesystem;
    if(fs::exists(p))
    {
        std::ifstream stream{p};
        std::string word;
        while(stream >> word)
        {
            QString qStrWord = QString::fromStdString(word).toUpper();
            if(qStrWord.length()==5)
            {
                m_set_of_words.insert(QString::fromStdString(word).toUpper());
            }
        }
    }
    else
    {
        throw std::logic_error("words.txt do not exists");
    }
}

GameState::GameState()
{
    namespace fs = std::filesystem;
    try
    {   const std::filesystem::path words{"/opt/wordle-task/words/"};
        for (auto const& dir_entry : fs::directory_iterator{words})
        {
            AddWordsFromFile(dir_entry);
        }

        if(m_set_of_words.size() == 0)
        {
            throw std::logic_error("set of words size is zero");
        }
        else
        {
            Reset();
        }
    }
    catch(std::exception& ex)
    {
        qDebug()<<QString("GameState std::exception(%1)\nExit 1").arg(ex.what());
        qApp->exit(1);
    }
};

void GameState::Reset()
{
    int pos = QRandomGenerator::global()->bounded(quint32(m_set_of_words.size()));
    m_word_hidden = *std::next(m_set_of_words.begin(), pos);
    qDebug() << "Word hidden:" << m_word_hidden;

    m_row = 0;

    for(size_t i = 0; i < ROWS_NUM; i++)
    {
        m_game_state_array[i] = QString();
        emit signalUpdateRowColors(i, ProcessRowColors(m_game_state_array[i]));
        emit signalUpdate(i);
    }

    emit signalReset();
}
