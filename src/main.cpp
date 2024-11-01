/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QDebug>
#include <QTreeView>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QApplication>
#include <QDesktopWidget>
#include <QFileSystemModel>
#include <QFileIconProvider>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QSortFilterProxyModel>

// our stuff
#include <src/model/custom-search-wrapper.h>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    QCommandLineParser parser;
    parser.setApplicationDescription("Qt Dir View Example");
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption dontUseCustomDirectoryIconsOption("c", "Set QFileIconProvider::DontUseCustomDirectoryIcons");
    parser.addOption(dontUseCustomDirectoryIconsOption);
    parser.addPositionalArgument("directory", "The directory to start in.");
    parser.process(app);

    // use home path if nothing is set otherwise
    const QString rootPath = parser.positionalArguments().isEmpty()
        ? QDir::homePath() : parser.positionalArguments().first();

    // add some more controls to support search
    auto window = new QWidget;
    auto edit = new QLineEdit;
    auto container = new QVBoxLayout{window};
    auto tree = new QTreeView;

    // insert our search wrapper here
    auto searchString = std::make_shared<QString>();
    auto model = new QFileSystemModel;
    auto wrapper = new test::CustomSearchWrapper{searchString};
    wrapper->setSourceModel(model);

    model->setRootPath("");
    if (parser.isSet(dontUseCustomDirectoryIconsOption)) {
        model->iconProvider()->setOptions(QFileIconProvider::DontUseCustomDirectoryIcons);
    }

    tree->setModel(wrapper);
    if (!rootPath.isEmpty()) {
        const QModelIndex rootIndex = model->index(QDir::cleanPath(rootPath));
        if (rootIndex.isValid()) {
            tree->setRootIndex(wrapper->mapFromSource(rootIndex));
        }
    }

    // set filter to include hidden files & (all) dirs
    model->setFilter(
        QDir::AllDirs | QDir::Hidden | QDir::Files | QDir::NoDot | QDir::NoDotDot
    );

    container->addWidget(edit);
    container->addWidget(tree);

    QObject::connect(edit, &QLineEdit::textChanged, [&searchString, tree](const QString& value) {
        *searchString = value.toLower();
        // only way to force resorting - changing order, otherwise sortByColumn does nothing.
        tree->sortByColumn(1, Qt::SortOrder::AscendingOrder);
        tree->sortByColumn(1, Qt::SortOrder::DescendingOrder);
    });

    const QSize availableSize = QApplication::desktop()->availableGeometry(tree).size();
    window->resize(availableSize / 2);
    window->setWindowTitle(QObject::tr("Dir View"));

    // Demonstrating look and feel features
    tree->setAnimated(false);
    tree->setIndentation(20);
    tree->setSortingEnabled(true);
    tree->setColumnWidth(0, tree->width() / 3);

    // we now show container instead
    window->show();

    int retval = app.exec();

    // cleanup
    delete wrapper;
    delete tree;
    delete edit;
    delete container;
    delete window;
    delete model;

    return retval;
}
