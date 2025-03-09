#pragma once
#pragma once
#include <QtHelper.h>
#include <request_check_the_row_body.pb.h>
#include <request_new_game_body.pb.h>
#include <response_check_the_row_body.pb.h>
#include <response_new_game_body.pb.h>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QUrl>
#include <enums/server_game.hpp>
#include <functional>
#include <type_traits>

using namespace wordle_data;

/// https://stackoverflow.com/questions/6512019/can-we-get-the-type-of-a-lambda-argument
template <typename F, typename RET, typename ARG>
ARG first_arg_lambda_helper(RET (F::*)(ARG) const);
template <typename LAMBDA_STRUCT>
struct first_arg_lambda {
  using type = std::remove_cvref_t<decltype(first_arg_lambda_helper(
      &LAMBDA_STRUCT::operator()))>;
};

class NetworkHelper : public QObject {
  Q_OBJECT

 public:
  template <typename CALLBACK_T, typename REQUEST_T>
  void SendRequest(const REQUEST_T& request_body,
                   QNetworkAccessManager::Operation operation,
                   const std::string& url, CALLBACK_T&& response_callback) {
    using RESPONSE_T = typename first_arg_lambda<CALLBACK_T>::type;

    [[maybe_unused]] bool connected;
    std::string serialized;
    request_body.SerializeToString(&serialized);

    QNetworkRequest request;
    request.setUrl(QUrl(QString::fromStdString(url)));
    request.setRawHeader(QByteArray("application/octet-stream"),
                         QByteArray(serialized.c_str()));
    QNetworkReply* reply = nullptr;
    switch (operation) {
      case QNetworkAccessManager::PostOperation {
        reply = network_access_manager.post(request); break;
      } default:
        break;
    }

    if (reply != nullptr) {
      connected = connect(reply, &QIODevice::readyRead, this, [&]() {
        QByteArray byte_array_response_serialized = reply->readAll();
        RESPONSE_T response;
        response.ParseFromString(byte_array_response_serialized.constData());
        response_callback(response);
      });
      IS_CONENCTED_OK
    }
  }

 private:
  QNetworkAccessManager network_access_manager;
};
