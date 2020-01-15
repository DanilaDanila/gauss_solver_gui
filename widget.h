#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QLabel>
#include <QTextEdit>
#include <QPushButton>

#include <vector>

class Widget : public QWidget
{
    Q_OBJECT

private:
    QVBoxLayout *main_layout;
    QHBoxLayout *pref_layout;
    QGridLayout *g_layout;
    /*
     * 0 - QLabel "Enter slau..."
     * 1 - aR/dR aC/dC
     * 2 - g_layout (SLAU)
     * 3 - btn solve
     * 4 - QLabel result
     */

    QLabel *enter_slau_msg_lbl;

    QPushButton *add_row_btn;
    QPushButton *del_row_btn;

    QPushButton *add_col_btn;
    QPushButton *del_col_btn;

    QPushButton *solve_btn;
    QLabel *result_lbl;

    unsigned edits_count = 3;
    unsigned equ_count = 2;
    std::vector<std::vector<QTextEdit*>> edits;
    std::vector<std::vector<QLabel*>> labels;

    void clearLayout(QLayout*, bool);

    void rebuildSLAU();

public:
    Widget(QWidget *parent = nullptr);

    ~Widget();

private slots:
    void add_row();
    void del_row();
    void add_col();
    void del_col();

    void solve();
};
#endif // WIDGET_H
