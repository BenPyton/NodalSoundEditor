/*
 * Copyright 2020 Benoit Pelletier
 *
 * This file is part of Sound Generator.
 *
 * Sound Generator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Sound Generator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Sound Generator.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#ifndef SELECTCOMPONENTCOMMAND
#define SELECTCOMPONENTCOMMAND

#include <QUndoCommand>
#include <QGraphicsScene>

class SelectComponentCommand : public QUndoCommand
{
public:
    SelectComponentCommand(QGraphicsItem* _item, bool _selected, QUndoCommand* _parent = nullptr);

    void undo() override;
    void redo() override;

private:
    QGraphicsItem* m_item;
    bool m_selected;
};

#endif // SELECTCOMPONENTCOMMAND
