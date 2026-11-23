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

#include "UBColorPaletteButton.h"

#include <QGridLayout>
#include <QMenu>
#include <QPainter>
#include <QPixmap>
#include <QWidgetAction>

#include "core/UBSettings.h"
#include "board/UBDrawingController.h"

#include "core/memcheck.h"

namespace
{
    const int kSwatchSize = 40;      // taille d'une pastille dans la grille (pixels), adaptée au tactile
    const int kButtonIconSize = 28;  // taille de l'icône du bouton dans la barre d'outils
}

UBColorPaletteButton::UBColorPaletteButton(QWidget *parent)
    : QToolButton(parent)
    , mMenu(nullptr)
    , mGridWidget(nullptr)
    , mCurrentIndex(0)
{
    setObjectName("ubColorPaletteButton");
    setPopupMode(QToolButton::InstantPopup);
    setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    setIconSize(QSize(kButtonIconSize, kButtonIconSize));

    mMenu = new QMenu(this);
    mMenu->setObjectName("ubColorPaletteMenu");

    mGridWidget = new QWidget(mMenu);
    QGridLayout* grid = new QGridLayout(mGridWidget);
    grid->setContentsMargins(6, 6, 6, 6);
    grid->setSpacing(4);

    const int count = sColumns * sRows;
    for (int i = 0; i < count; i++)
    {
        QToolButton* swatch = new QToolButton(mGridWidget);
        swatch->setObjectName("ubColorSwatch");
        swatch->setAutoRaise(true);
        swatch->setFixedSize(kSwatchSize + 8, kSwatchSize + 8);
        swatch->setIconSize(QSize(kSwatchSize, kSwatchSize));
        swatch->setProperty("swatchIndex", i);
        connect(swatch, SIGNAL(clicked()), this, SLOT(swatchClicked()));
        grid->addWidget(swatch, i / sColumns, i % sColumns);
        mSwatches.append(swatch);
    }

    QWidgetAction* gridAction = new QWidgetAction(mMenu);
    gridAction->setDefaultWidget(mGridWidget);
    mMenu->addAction(gridAction);

    setMenu(mMenu);
}

UBColorPaletteButton::~UBColorPaletteButton()
{
    // NOOP : les enfants sont détruits avec le parent Qt
}

void UBColorPaletteButton::setLabel(const QString& label)
{
    mLabel = label;
    setText(label);
    setToolTip(label);
}

void UBColorPaletteButton::displayText(QVariant display)
{
    setToolButtonStyle(display.toBool() ? Qt::ToolButtonTextUnderIcon : Qt::ToolButtonIconOnly);
}

void UBColorPaletteButton::setCurrentIndex(int index)
{
    if (index < 0 || index >= mColors.size())
        return;

    if (index != mCurrentIndex)
    {
        mCurrentIndex = index;
        rebuildSwatches();
    }
    else
    {
        updateButtonIcon();
    }
}

void UBColorPaletteButton::colorPaletteChanged()
{
    UBSettings* settings = UBSettings::settings();
    bool isDarkBackground = settings->isDarkBackground();
    int tool = UBDrawingController::drawingController()->stylusTool();

    if (tool == UBStylusTool::Marker)
    {
        mColors = settings->markerColors(isDarkBackground);
        mCurrentIndex = settings->markerColorIndex();
    }
    else
    {
        // outil crayon, ligne, texte, etc. : couleurs du stylo
        mColors = settings->penColors(isDarkBackground);
        mCurrentIndex = settings->penColorIndex();
    }

    if (mCurrentIndex < 0 || mCurrentIndex >= mColors.size())
        mCurrentIndex = 0;

    rebuildSwatches();
}

void UBColorPaletteButton::swatchClicked()
{
    QToolButton* swatch = qobject_cast<QToolButton*>(sender());
    if (!swatch)
        return;

    int index = swatch->property("swatchIndex").toInt();
    if (index < 0 || index >= mColors.size())
        return;

    mMenu->hide();
    emit activated(index);
}

void UBColorPaletteButton::rebuildSwatches()
{
    for (int i = 0; i < mSwatches.size(); i++)
    {
        QToolButton* swatch = mSwatches.at(i);
        bool valid = i < mColors.size();
        swatch->setEnabled(valid);

        if (valid)
        {
            swatch->setIcon(swatchIcon(mColors.at(i), i == mCurrentIndex));
            swatch->setToolTip(mColors.at(i).name(QColor::HexRgb).toUpper());
        }
        else
        {
            swatch->setIcon(QIcon());
            swatch->setToolTip(QString());
        }
    }

    updateButtonIcon();
}

void UBColorPaletteButton::updateButtonIcon()
{
    if (mCurrentIndex >= 0 && mCurrentIndex < mColors.size())
    {
        setIcon(swatchIcon(mColors.at(mCurrentIndex), false));
    }
}

QIcon UBColorPaletteButton::swatchIcon(const QColor& color, bool selected) const
{
    QPixmap pixmap(kSwatchSize, kSwatchSize);
    pixmap.fill(Qt::transparent);

    QColor opaque = color;
    opaque.setAlpha(255);   // aperçu opaque, y compris pour le marqueur (qui est semi-transparent)

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QRectF rect(3, 3, kSwatchSize - 6, kSwatchSize - 6);
    painter.setPen(QPen(QColor(90, 90, 90), 1));
    painter.setBrush(opaque);
    painter.drawRoundedRect(rect, 5, 5);

    if (selected)
    {
        // anneau de sélection : noir puis blanc pour rester visible sur toutes les couleurs
        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen(Qt::black, 2));
        painter.drawRoundedRect(QRectF(1, 1, kSwatchSize - 2, kSwatchSize - 2), 6, 6);
        painter.setPen(QPen(Qt::white, 1));
        painter.drawRoundedRect(QRectF(3, 3, kSwatchSize - 6, kSwatchSize - 6), 5, 5);
    }

    painter.end();
    return QIcon(pixmap);
}
