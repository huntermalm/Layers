#include "../../include/Downloader.h"

#include <QDir>
#include <QSaveFile>

using Layers::Downloader;

Downloader::Downloader(QObject* parent) :
	QObject(parent)
{
}

QNetworkReply* Downloader::download(const QUrl& file_url, const QDir& directory)
{
	QNetworkRequest request(file_url);

	QNetworkReply* reply = m_network_manager.get(request);

	connect(reply, &QNetworkReply::finished, [=] {

		if (reply->error() == QNetworkReply::NoError)
		{
			QSaveFile file(directory.filePath(file_url.fileName()));

			file.open(QIODevice::WriteOnly);
			file.write(reply->readAll());
			file.commit();
		}

		reply->deleteLater();
	});

	return reply;
}

QNetworkReply* Downloader::download(const QUrl& file_url)
{
	QNetworkRequest request(file_url);

	QNetworkReply* reply = m_network_manager.get(request);

	connect(reply, &QNetworkReply::finished, [=] {
		reply->deleteLater();
	});

	return reply;
}
