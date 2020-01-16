#include "widget.h"

#include <iostream>

#include <string>

#include "solver/fract.hpp"
#include "solver/slau.hpp"

#include <sstream>

Widget::Widget(QWidget *parent): QWidget(parent)
{
    setFixedWidth(500);

    main_layout = new QVBoxLayout(this);
    main_layout->setAlignment(Qt::AlignCenter);
        enter_slau_msg_lbl = new QLabel(this);
        enter_slau_msg_lbl->setText(tr("Введите СЛАУ:"));
        enter_slau_msg_lbl->setFixedHeight(40);
        main_layout->addWidget(enter_slau_msg_lbl, 0);

        pref_layout = new QHBoxLayout();
        pref_layout->setAlignment(Qt::AlignCenter);
            add_row_btn = new QPushButton(this);
            del_row_btn = new QPushButton(this);
            add_col_btn = new QPushButton(this);
            del_col_btn = new QPushButton(this);

            add_row_btn->setText(tr("+row"));
            del_row_btn->setText(tr("-row"));
            add_col_btn->setText(tr("+col"));
            del_col_btn->setText(tr("-col"));

            add_row_btn->setFixedSize(60, 36);
            del_row_btn->setFixedSize(60, 36);
            add_col_btn->setFixedSize(60, 36);
            del_col_btn->setFixedSize(60, 36);

            connect(add_row_btn, SIGNAL(released()), this, SLOT(add_row()));
            connect(del_row_btn, SIGNAL(released()), this, SLOT(del_row()));
            connect(add_col_btn, SIGNAL(released()), this, SLOT(add_col()));
            connect(del_col_btn, SIGNAL(released()), this, SLOT(del_col()));

            pref_layout->addWidget(add_row_btn, 0);
            pref_layout->addWidget(del_row_btn, 1);
            pref_layout->addWidget(add_col_btn, 2);
            pref_layout->addWidget(del_col_btn, 3);
        main_layout->addLayout(pref_layout, 1);

        g_layout = new QGridLayout();
        g_layout->setAlignment(Qt::AlignCenter);
        main_layout->addLayout(g_layout, 2);

        solve_btn = new QPushButton(this);
        solve_btn->setMaximumHeight(40);
        solve_btn->setText(tr("Решить СЛАУ"));
        connect(solve_btn, SIGNAL(released()), this, SLOT(solve()));
        main_layout->addWidget(solve_btn, 3);

        result_lbl = new QLabel(this);
        result_lbl->setVisible(false);
        main_layout->addWidget(result_lbl, 4);

      rebuildSLAU();
}

// TODO:
Widget::~Widget() {}

void Widget::clearLayout(QLayout* layout, bool deleteWidgets = true)
{
    while (QLayoutItem* item = layout->takeAt(0))
    {
        if (deleteWidgets)
        {
            if (QWidget* widget = item->widget())
                widget->deleteLater();
        }
        if (QLayout* childLayout = item->layout())
            clearLayout(childLayout, deleteWidgets);
        delete item;
    }
}

void Widget::rebuildSLAU()
{
    clearLayout(g_layout);

    for(int i=0; i<edits.size(); ++i)
        for(int j=0; j<edits[i].size(); ++j)
            delete edits[i][j];

    for(int i=0; i<labels.size(); ++i)
        for(int j=0; j<labels[i].size(); ++j)
            delete labels[i][j];

    edits.clear();
    labels.clear();

    for(unsigned i=0; i<equ_count; ++i)
    {
        edits.push_back(std::vector<QTextEdit*>());
        labels.push_back(std::vector<QLabel*>());

        for(unsigned j=0; j<edits_count; ++j)
        {
            QTextEdit *edt = new QTextEdit(this);
            edt->setFixedSize(36, 26);
            edits[i].push_back(edt);

            g_layout->addWidget(edt, i, 2*j);
        }

        for(unsigned j=0; j<edits_count-1; ++j)
        {
            QLabel *label = new QLabel(this);
            std::string str = "*X" + std::to_string(j+1);
            if(j == edits_count-2)
                str += "=";
            else
                str += "+";
            label->setText(tr(str.c_str()));
            label->setFixedSize(30, 26);
            labels[i].push_back(label);

            g_layout->addWidget(label, i, 2*j+1);
        }
    }
}

void Widget::add_row() { if(equ_count < 10) ++equ_count; rebuildSLAU(); }

void Widget::del_row() { if(equ_count > 2) --equ_count; rebuildSLAU(); }

void Widget::add_col() { if(edits_count < 10) ++edits_count; rebuildSLAU(); }

void Widget::del_col() { if(edits_count > 3) --edits_count; rebuildSLAU(); }

void Widget::solve()
{
    for(unsigned i=0; i<edits.size(); ++i)
        for(unsigned j=0; j<edits[i].size(); ++j)
            if(edits[i][j]->toPlainText().isNull())
                return;

    Vector<Vector<int>> slau;

    for(unsigned i=0; i<edits.size(); ++i)
    {
        Vector<int> equ;
        for(unsigned j=0; j<edits[i].size(); ++j)
            equ.append(edits[i][j]->toPlainText().toInt());
        slau.append(equ);
    }

    std::cerr<<as_slau(slau)<<"\n";
    Vector<Vector<Fract>> solution = get_solution(slau);
    std::cerr<<as_matrix(solution)<<"\n";

    std::ostringstream out;
    out<<"Ответ:\n\n";

    if(solution.size() == 0)
    {
        out<<"Решений нет";
    }
    else
    {
        out<<"СЛАУ:\n"
           <<as_slau(slau)<<"\n"

           <<"Расширенная матрица СЛАУ:\n"
           <<as_extended_matrix(slau)<<"\n"

           <<"Приведённая СЛАУ:\n"
           <<as_extended_matrix(triangulate(slau))<<"\n"

           <<"Решение в матричном виде:\n"
           <<as_matrix(solution)<<"\n";

        out<<"Решение в виде суммы векторов:\n";
        Vector<Vector<Fract>> solution_T = transpose(solution);
        for(unsigned i=0; i<solution_T.size(); ++i)
        {
            Vector<Vector<Fract>> line;
            line.append(solution_T[i]);
            std::cerr<<line<<"\n";

            if(i != 0)
                out<<" + c"<<i<<"*";

            out<<as_matrix(line);
        }
    }
    result_lbl->setVisible(true);
    result_lbl->setText(tr(out.str().c_str()));
}
