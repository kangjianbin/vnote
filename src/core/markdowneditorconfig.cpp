#include "markdowneditorconfig.h"

#include <QDebug>

#include "texteditorconfig.h"
#include "mainconfig.h"

using namespace vnotex;

#define READSTR(key) readString(appObj, userObj, (key))
#define READBOOL(key) readBool(appObj, userObj, (key))
#define READREAL(key) readReal(appObj, userObj, (key))
#define READINT(key) readInt(appObj, userObj, (key))

MarkdownEditorConfig::MarkdownEditorConfig(ConfigMgr *p_mgr,
                                           IConfig *p_topConfig,
                                           const QSharedPointer<TextEditorConfig> &p_textEditorConfig)
    : IConfig(p_mgr, p_topConfig),
      m_textEditorConfig(p_textEditorConfig)
{
    m_sessionName = QStringLiteral("markdown_editor");
}

void MarkdownEditorConfig::init(const QJsonObject &p_app, const QJsonObject &p_user)
{
    const auto appObj = p_app.value(m_sessionName).toObject();
    const auto userObj = p_user.value(m_sessionName).toObject();

    loadViewerResource(appObj, userObj);

    m_webPlantUml = READBOOL(QStringLiteral("web_plantuml"));
    m_webGraphviz = READBOOL(QStringLiteral("web_graphviz"));

    m_prependDotInRelativeLink = READBOOL(QStringLiteral("prepend_dot_in_relative_link"));
    m_confirmBeforeClearObsoleteImages = READBOOL(QStringLiteral("confirm_before_clear_obsolete_images"));
    m_insertFileNameAsTitle = READBOOL(QStringLiteral("insert_file_name_as_title"));

    m_sectionNumberMode = stringToSectionNumberMode(READSTR(QStringLiteral("section_number")));
    m_sectionNumberBaseLevel = READINT(QStringLiteral("section_number_base_level"));

    m_constrainImageWidthEnabled = READBOOL(QStringLiteral("constrain_image_width"));
    m_constrainInPlacePreviewWidthEnabled = READBOOL(QStringLiteral("constrain_inplace_preview_width"));
    m_zoomFactorInReadMode = READREAL(QStringLiteral("zoom_factor_in_read_mode"));
    m_fetchImagesInParseAndPaste = READBOOL(QStringLiteral("fetch_images_in_parse_and_paste"));
    m_protectFromXss = READBOOL(QStringLiteral("protect_from_xss"));
    m_htmlTagEnabled = READBOOL(QStringLiteral("html_tag"));
    m_autoBreakEnabled = READBOOL(QStringLiteral("auto_break"));
    m_linkifyEnabled = READBOOL(QStringLiteral("linkify"));
}

QJsonObject MarkdownEditorConfig::toJson() const
{
    QJsonObject obj;
    obj[QStringLiteral("viewer_resource")] = saveViewerResource();
    obj[QStringLiteral("web_plantuml")] = m_webPlantUml;
    obj[QStringLiteral("web_graphviz")] = m_webGraphviz;
    obj[QStringLiteral("prepend_dot_in_relative_link")] = m_prependDotInRelativeLink;
    obj[QStringLiteral("confirm_before_clear_obsolete_images")] = m_confirmBeforeClearObsoleteImages;
    obj[QStringLiteral("insert_file_name_as_title")] = m_insertFileNameAsTitle;

    obj[QStringLiteral("section_number")] = sectionNumberModeToString(m_sectionNumberMode);
    obj[QStringLiteral("section_number_base_level")] = m_sectionNumberBaseLevel;

    obj[QStringLiteral("constrain_image_width")] = m_constrainImageWidthEnabled;
    obj[QStringLiteral("constrain_inplace_preview_width")] = m_constrainInPlacePreviewWidthEnabled;
    obj[QStringLiteral("zoom_factor_in_read_mode")] = m_zoomFactorInReadMode;
    obj[QStringLiteral("fetch_images_in_parse_and_paste")] = m_fetchImagesInParseAndPaste;
    obj[QStringLiteral("protect_from_xss")] = m_protectFromXss;
    obj[QStringLiteral("html_tag")] = m_htmlTagEnabled;
    obj[QStringLiteral("auto_break")] = m_autoBreakEnabled;
    obj[QStringLiteral("linkify")] = m_linkifyEnabled;
    return obj;
}

TextEditorConfig &MarkdownEditorConfig::getTextEditorConfig()
{
    return *m_textEditorConfig;
}

const TextEditorConfig &MarkdownEditorConfig::getTextEditorConfig() const
{
    return *m_textEditorConfig;
}

int MarkdownEditorConfig::revision() const
{
    return m_revision + m_textEditorConfig->revision();
}

void MarkdownEditorConfig::loadViewerResource(const QJsonObject &p_app, const QJsonObject &p_user)
{
    const QString name(QStringLiteral("viewer_resource"));

    if (MainConfig::isVersionChanged()) {
        bool needOverride = p_app[QStringLiteral("override_viewer_resource")].toBool();
        if (needOverride) {
            qInfo() << "override \"viewer_resource\" in user configuration due to version change";
            m_viewerResource.init(p_app[name].toObject());
            return;
        }
    }

    if (p_user.contains(name)) {
        m_viewerResource.init(p_user[name].toObject());
    } else {
        m_viewerResource.init(p_app[name].toObject());
    }
}

QJsonObject MarkdownEditorConfig::saveViewerResource() const
{
    return m_viewerResource.toJson();
}

const ViewerResource &MarkdownEditorConfig::getViewerResource() const
{
    return m_viewerResource;
}

bool MarkdownEditorConfig::getWebPlantUml() const
{
    return m_webPlantUml;
}

bool MarkdownEditorConfig::getWebGraphviz() const
{
    return m_webGraphviz;
}

bool MarkdownEditorConfig::getPrependDotInRelativeLink() const
{
    return m_prependDotInRelativeLink;
}

bool MarkdownEditorConfig::getConfirmBeforeClearObsoleteImages() const
{
    return m_confirmBeforeClearObsoleteImages;
}

void MarkdownEditorConfig::setConfirmBeforeClearObsoleteImages(bool p_confirm)
{
    updateConfig(m_confirmBeforeClearObsoleteImages,
                 p_confirm,
                 this);
}

bool MarkdownEditorConfig::getInsertFileNameAsTitle() const
{
    return m_insertFileNameAsTitle;
}

void MarkdownEditorConfig::setInsertFileNameAsTitle(bool p_enabled)
{
    updateConfig(m_insertFileNameAsTitle, p_enabled, this);
}

bool MarkdownEditorConfig::getConstrainImageWidthEnabled() const
{
    return m_constrainImageWidthEnabled;
}

void MarkdownEditorConfig::setConstrainImageWidthEnabled(bool p_enabled)
{
    updateConfig(m_constrainImageWidthEnabled, p_enabled, this);
}

bool MarkdownEditorConfig::getConstrainInPlacePreviewWidthEnabled() const
{
    return m_constrainInPlacePreviewWidthEnabled;
}

void MarkdownEditorConfig::setConstrainInPlacePreviewWidthEnabled(bool p_enabled)
{
    updateConfig(m_constrainInPlacePreviewWidthEnabled, p_enabled, this);
}

qreal MarkdownEditorConfig::getZoomFactorInReadMode() const
{
    return m_zoomFactorInReadMode;
}

void MarkdownEditorConfig::setZoomFactorInReadMode(qreal p_factor)
{
    updateConfig(m_zoomFactorInReadMode, p_factor, this);
}

bool MarkdownEditorConfig::getFetchImagesInParseAndPaste() const
{
    return m_fetchImagesInParseAndPaste;
}

void MarkdownEditorConfig::setFetchImagesInParseAndPaste(bool p_enabled)
{
    updateConfig(m_fetchImagesInParseAndPaste, p_enabled, this);
}

bool MarkdownEditorConfig::getProtectFromXss() const
{
    return m_protectFromXss;
}

bool MarkdownEditorConfig::getHtmlTagEnabled() const
{
    return m_htmlTagEnabled;
}

void MarkdownEditorConfig::setHtmlTagEnabled(bool p_enabled)
{
    updateConfig(m_htmlTagEnabled, p_enabled, this);
}

bool MarkdownEditorConfig::getAutoBreakEnabled() const
{
    return m_autoBreakEnabled;
}

void MarkdownEditorConfig::setAutoBreakEnabled(bool p_enabled)
{
    updateConfig(m_autoBreakEnabled, p_enabled, this);
}

bool MarkdownEditorConfig::getLinkifyEnabled() const
{
    return m_linkifyEnabled;
}

void MarkdownEditorConfig::setLinkifyEnabled(bool p_enabled)
{
    updateConfig(m_linkifyEnabled, p_enabled, this);
}

QString MarkdownEditorConfig::sectionNumberModeToString(SectionNumberMode p_mode) const
{
    switch (p_mode) {
    case SectionNumberMode::None:
        return QStringLiteral("none");

    case SectionNumberMode::Edit:
        return QStringLiteral("edit");

    default:
        return QStringLiteral("read");
    }
}

MarkdownEditorConfig::SectionNumberMode MarkdownEditorConfig::stringToSectionNumberMode(const QString &p_str) const
{
    auto mode = p_str.toLower();
    if (mode == QStringLiteral("none")) {
        return SectionNumberMode::None;
    } else if (mode == QStringLiteral("edit")) {
        return SectionNumberMode::Edit;
    } else {
        return SectionNumberMode::Read;
    }
}

MarkdownEditorConfig::SectionNumberMode MarkdownEditorConfig::getSectionNumberMode() const
{
    return m_sectionNumberMode;
}

void MarkdownEditorConfig::setSectionNumberMode(SectionNumberMode p_mode)
{
    updateConfig(m_sectionNumberMode, p_mode, this);
}

int MarkdownEditorConfig::getSectionNumberBaseLevel() const
{
    return m_sectionNumberBaseLevel;
}

void MarkdownEditorConfig::setSectionNumberBaseLevel(int p_level)
{
    updateConfig(m_sectionNumberBaseLevel, p_level, this);
}
