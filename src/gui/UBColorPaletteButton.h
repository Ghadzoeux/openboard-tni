/*
 * Copyright (C) 2015-2022 Département de l'Instruction Publique (DIP-SEM)
 *
 * Copyright (C) 2013 Open Education Foundation
 *
 * Copyright (C) 2010-2013 Groupement d'Intérêt Public pour
 * l'Education Numérique en Afrique (GIP ENA)
 *
 * This file is part of OpenBoard.
 *
 * OpenBoard is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License,
 * with a specific linking exception for the OpenSSL project's
 * "OpenSSL" library (or with modified versions of it that use the
 * same license as the "OpenSSL" library).
 *
 * OpenBoard is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenBoard. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef UBCOLORPALETTEBUTTON_H_
#define UBCOLORPALETTEBUTTON_H_

#include <QList>
#include <QColor>
#include <QToolButton>
#include <QVariant>

class QMenu;
class QWidget;

/**
 * openboard-tni : bouton de couleur de la barre d'outils du tableau.
 *
 * Remplace le groupe de 5 boutons fixes par un seul bouton affichant la couleur courante.
 * Un clic ouvre une grille de 2 lignes x 8 colonnes avec les 16 couleurs de la palette.
 */
class UBColorPaletteButton : public QToolButton
{
    Q_OBJECT

    public:
        explicit UBColorPaletteButton(QWidget *parent = nullptr);
        virtual ~UBColorPaletteButton();

        void setLabel(const QString& label);
        int currentIndex() const { return mCurrentIndex; }

        static const int sColumns = 8;
        static const int sRows = 2;

    public slots:
        void setCurrentIndex(int index);
        void colorPaletteChanged();
        void displayText(QVariant display);

    signals:
        void activated(int index);

    private slots:
        void swatchClicked();

    private:
        void rebuildSwatches();
        void updateButtonIcon();
        QIcon swatchIcon(const QColor& color, bool selected) const;

        QMenu*               mMenu;
        QWidget*             mGridWidget;
        QList<QToolButton*>  mSwatches;
        QList<QColor>        mColors;
        int                  mCurrentIndex;
        QString              mLabel;
};

#endif /* UBCOLORPALETTEBUTTON_H_ */
