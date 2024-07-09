#include "usermanageview.h"
#include "ui_usermanageview.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

UserManageView::UserManageView(QWidget *parent) :
    QWidget(parent)
    //ui(new Ui::UserManageView)
{
    //ui->setupUi(this);

    QVBoxLayout *layout = new QVBoxLayout(this);

        QLabel *titleLabel = new QLabel("User Management", this);
        titleLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(titleLabel);

        usernameEdit = new QLineEdit(this);
        usernameEdit->setPlaceholderText("Enter username");
        layout->addWidget(usernameEdit);

        addButton = new QPushButton("Add User", this);
        layout->addWidget(addButton);


        QHBoxLayout *headerLayout = new QHBoxLayout();

        QLabel *idLabel = new QLabel("1. ID", this);
        headerLayout->addWidget(idLabel);

        QLabel *usernameLabel = new QLabel("2. Username", this);
        headerLayout->addWidget(usernameLabel);

        QLabel *roleLabel = new QLabel("3. Role", this);
        headerLayout->addWidget(roleLabel);

        QLabel *lastTimeLabel = new QLabel("4. Last Time", this);
        headerLayout->addWidget(lastTimeLabel);

        layout->addLayout(headerLayout);


        userTable = new QTableWidget(4, 4, this);
        userTable->setHorizontalHeaderLabels({"ID", "Username", "password", "Last Time", "", "", ""}); // Adjusted headers
        layout->addWidget(userTable);

        setLayout(layout);
}

UserManageView::~UserManageView()
{
    //delete ui;
}
