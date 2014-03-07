#ifndef CELLULARDIALOG_H
#define CELLULARDIALOG_H

/*
 * Copyright 2013 Miloslav Číž
 *
 * This file is part of PT Designer.
 *
 * PT Designer is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PT Designer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with these files. If not, see <http://www.gnu.org/licenses/>.
 */

#include <QDialog>
#include "customblockdialog.h"

namespace Ui {
class CellularDialog;
}

/**
 * A dialog for cyclic and RPS cellular automaton.
 */

class CellularDialog : public CustomBlockDialog
{
    Q_OBJECT

public:
    explicit CellularDialog(c_block *block, QWidget *parent);
    ~CellularDialog();

private slots:
    void on_buttonBox_accepted();

    void on_button_set_clicked();

private:
    Ui::CellularDialog *ui;
};

#endif // CELLULARDIALOG_H
