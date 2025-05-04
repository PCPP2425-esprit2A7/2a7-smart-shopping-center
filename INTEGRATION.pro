QT += core gui widgets sql charts network serialport multimedia multimediawidgets pdf websockets printsupport

CONFIG += c++17
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    chatwindow.cpp \
    clickablerectitem.cpp \
    dimensionsdialog.cpp \
    espace.cpp \
    espace3dview.cpp \
    event.cpp \
    mailjetsender.cpp \
    mailsender.cpp \
    main.cpp \
    maintenance.cpp \
    mainwindow.cpp \
    connection.cpp \
    employe.cpp \
    facelogindialog.cpp \
    chatmanager.cpp \
    arduino.cpp \
    openaiclient.cpp \
    openaihelper.cpp \
    service.cpp

HEADERS += \
    chatwindow.h \
    clickablerectitem.h \
    dimensionsdialog.h \
    espace.h \
    espace3dview.h \
    event.h \
    mailjetsender.h \
    mailsender.h \
    maintenance.h \
    mainwindow.h \
    connection.h \
    employe.h \
    facelogindialog.h \
    chatmanager.h \
    arduino.h \
    openaiclient.h \
    openaihelper.h \
    service.h

FORMS += \
    chatwindow.ui \
    dimensionsdialog.ui \
    mainwindow.ui \
    facelogindialog.ui

RESOURCES += \
    img.qrc

DISTFILES += \
    img/-brightness-3_90399.png \
    img/.png \
    img/11 (2).png \
    img/1491254405-plusaddmoredetail_82972.png \
    img/1491254488-chartflexibledatestatstatistics_82950.png \
    img/1623971-removebg-preview.png \
    img/1904660-email-envelope-letter-mail-message-post-send_122510 (1).png \
    img/3079026-removebg-preview.png \
    img/360_F_565224180_QNRiRQkf9Fw0dKRoZGwUknmmfk51SuSS-removebg-preview.png \
    img/4124799-finance-report-graph-analysis-graph-report-sale-report-stock-report_113890.png \
    img/4986021-removebg-preview.png \
    img/5973800-removebg-preview.png \
    img/9321345-removebg-preview.png \
    img/Design sans titre (3).png \
    img/Instagram_New_icon-icons.com_69008.png \
    img/ai_artificial_intelligence_robot_chip_brain_technology_icon_179495.png \
    img/artificial_intelligence-ai-head-robot-512-removebg-preview.png \
    img/bouton-ajouter.png \
    img/brightness_icon_128445.png \
    img/calendar_date_day_event_month_schedule_time_icon_123230.png \
    img/ecommerce_home_market_mart_shop_shopping_store_icon_123207.png \
    img/employee_group_solid_icon_235378.png \
    img/excel_office_4658.png \
    img/export.sql \
    img/fb_icon-icons.com_65434.png \
    img/graphique.png \
    img/investment_asset_management_worker_employee_welfare_icon_210504.png \
    img/liste.png \
    img/logo_final-removebg-preview (1).ico \
    img/logo_final-removebg-preview.ico \
    img/logo_final-removebg-preview.png \
    img/logo_final.jpg \
    img/notification_4418844.png \
    img/pdf-file-outlined-interface-symbol_icon-icons.com_57528.png \
    img/pdf.png \
    img/robot-round-head_icon-icons.com_55274.png \
    img/sidebar_utilities_18421.png \
    img/sms-speech-bubble_icon-icons.com_72541.png \
    img/suivi (1).png \
    img/suivi.png \
    img/tel.png \
    img/validated_buyer_user_apply_done_icon_177174 (1).png \
    img/website_maintenance_settings_window_alert_icon_193897 (1).png

# 🔧 Spécial MinGW : forcer le linkage avec Qt3D libs (si nécessaire)
win32:CONFIG(release, debug|release): LIBS += -L$$[QT_INSTALL_LIBS] \
    -lQt63DCore -lQt63DRender -lQt63DInput -lQt63DLogic -lQt63DExtras -lQt63DAnimation

# 🧩 Ajout pour s'assurer que tous les includes sont trouvés
INCLUDEPATH += $$[QT_INSTALL_HEADERS]

# 📦 Déploiement
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
