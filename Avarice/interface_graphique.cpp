#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QInputDialog>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>

class Server : public QWidget {
    Q_OBJECT
public:
    explicit Server(QWidget *parent = nullptr) : QWidget(parent) {
        server = new QTcpServer(this);
        if (!server->listen(QHostAddress::Any, 12345)) {
            QMessageBox::critical(this, "Error", "Unable to start the server.");
            return;
        }

        connect(server, &QTcpServer::newConnection, this, &Server::newConnection);

        clientsListWidget = new QListWidget(this);
        connect(clientsListWidget, &QListWidget::itemClicked, this, &Server::clientSelected);

        QPushButton *disconnectButton = new QPushButton("Disconnect", this);
        connect(disconnectButton, &QPushButton::clicked, this, &Server::disconnectClient);

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(clientsListWidget);
        layout->addWidget(disconnectButton);

        setLayout(layout);
    }

private slots:
    void newConnection() {
        QTcpSocket *clientSocket = server->nextPendingConnection();
        clients.insert(clientSocket);
        connect(clientSocket, &QTcpSocket::readyRead, this, &Server::readData);
        connect(clientSocket, &QTcpSocket::disconnected, this, &Server::clientDisconnected);

        QListWidgetItem *item = new QListWidgetItem(clientSocket->peerAddress().toString(), clientsListWidget);
        clientsListWidget->addItem(item);
    }

    void clientSelected(QListWidgetItem *item) {
        if (!item) return;
        currentClient = dynamic_cast<QTcpSocket*>(clientsListWidget->currentItem());
    }

    void disconnectClient() {
        if (!currentClient) return;
        currentClient->disconnectFromHost();
    }

    void readData() {
        QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
        if (!clientSocket) return;

        QByteArray data = clientSocket->readAll();
        // Traitement des données reçues et envoi des réponses
        // Exemple de traitement : renvoyer les données telles quelles
        clientSocket->write(data);
    }

    void clientDisconnected() {
        QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
        if (!clientSocket) return;

        clients.remove(clientSocket);
        for (int i = 0; i < clientsListWidget->count(); ++i) {
            QListWidgetItem *item = clientsListWidget->item(i);
            if (item && item->text() == clientSocket->peerAddress().toString()) {
                delete item;
                break;
            }
        }
        clientSocket->deleteLater();
    }

private:
    QTcpServer *server;
    QList<QTcpSocket*> clients;
    QListWidget *clientsListWidget;
    QTcpSocket *currentClient = nullptr;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Server serverWindow;
    serverWindow.setWindowTitle("Server");
    serverWindow.show();

    return app.exec();
}

#include "main.moc"
