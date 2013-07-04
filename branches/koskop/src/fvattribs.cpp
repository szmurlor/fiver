/*****************************************************************************
 *   Copyright (C) 2008 Robert Szmurlo <robert@iem.pw.edu.pl>                *
 *                                                                           *
 *   Licensed under the Apache License, Version 2.0 (the "License");         *
 *   you may not use this file except in compliance with the License.        *
 *   You may obtain a copy of the License at                                 *
 *                                                                           *
 *      http://www.apache.org/licenses/LICENSE-2.0                           *
 *                                                                           *
 *   Unless required by applicable law or agreed to in writing, software     *
 *   distributed under the License is distributed on an "AS IS" BASIS,       *
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.*
 *   See the License for the specific language governing permissions and     *
 *   limitations under the License.                                          *
 *****************************************************************************/

#include "fvattribs.h"
#include <QBoxLayout>
#include <QPushButton>

FVAttribs::FVAttribs(QWidget *parent)
 : QWidget(parent)
{
    setupUI();
}


FVAttribs::~FVAttribs()
{
}

void FVAttribs::setupUI()
{
    QBoxLayout *box = new QBoxLayout(QBoxLayout::TopToBottom, this);
    setLayout(box);
    
    at = new AttrTable(this);
    connect( at, SIGNAL(signalApply()), this, SLOT( slotApplyAttribs() ) );
    box->addWidget(at,1);


    QPushButton * cmd = new QPushButton("Apply",this);
    connect( cmd, SIGNAL(clicked()), this, SLOT( slotApplyAttribs() ) );
    box->addWidget(cmd,0);

}

void FVAttribs::slotApplyAttribs()
{
    emit signalApply();
}

void FVAttribs::setModel(AttrModel * model)
{
    at->setModel( model );
}


