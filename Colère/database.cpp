#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStringList>
#include <QVariant>

class LPTF_Database {
public:
    LPTF_Database(const QString &dbName) {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(dbName);

        if (!db.open()) {
            qWarning() << "Failed to open database:" << db.lastError().text();
        } else {
            createTables();
        }
    }

    void insertClient(const QString &ipAddress, bool online) {
        QSqlQuery query;
        query.prepare("INSERT INTO clients (ip_address, online) VALUES (:ip_address, :online)");
        query.bindValue(":ip_address", ipAddress);
        query.bindValue(":online", online);
        if (!query.exec()) {
            qWarning() << "Failed to insert client:" << query.lastError().text();
        }
    }

    void updateClientStatus(const QString &ipAddress, bool online) {
        QSqlQuery query;
        query.prepare("UPDATE clients SET online = :online WHERE ip_address = :ip_address");
        query.bindValue(":online", online);
        query.bindValue(":ip_address", ipAddress);
        if (!query.exec()) {
            qWarning() << "Failed to update client status:" << query.lastError().text();
        }
    }

    QStringList getOnlineClients() {
        QStringList onlineClients;
        QSqlQuery query("SELECT ip_address FROM clients WHERE online = 1");
        while (query.next()) {
            onlineClients.append(query.value(0).toString());
        }
        return onlineClients;
    }

    void insertResponse(const QString &ipAddress, const QString &response) {
        QSqlQuery query;
        query.prepare("INSERT INTO responses (ip_address, response) VALUES (:ip_address, :response)");
        query.bindValue(":ip_address", ipAddress);
        query.bindValue(":response", response);
        if (!query.exec()) {
            qWarning() << "Failed to insert response:" << query.lastError().text();
        }
    }

private:
    void createTables() {
        QSqlQuery query;
        query.exec("CREATE TABLE IF NOT EXISTS clients (ip_address TEXT PRIMARY KEY, online BOOLEAN)");
        query.exec("CREATE TABLE IF NOT EXISTS responses (id INTEGER PRIMARY KEY AUTOINCREMENT, ip_address TEXT, response TEXT)");
    }

    QSqlDatabase db;
};
