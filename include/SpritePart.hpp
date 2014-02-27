#ifndef SSPRITEPART_HPP
#define SSPRITEPART_HPP

#include <Types.hpp>
#ifndef LIBZELDA_USE_QT
#   include <vector>
#else
#   include <QObject>
#   include <QPoint>
#   include <QSize>
#   include <QString>
#endif

#include <string>
#include <Types.hpp>

namespace zelda
{
namespace Sakura
{
class SpriteFrame;

#ifndef LIBZELDA_USE_QT
class SpritePart
{
#else
class SpritePart : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(bool hasCollision READ hasCollision WRITE setCollision)
    Q_PROPERTY(bool flippedHorizontally READ flippedHorizontally WRITE setFlippedHorizontally)
    Q_PROPERTY(bool flippedVertically READ flippedVertically WRITE setFlippedVertically)
    Q_PROPERTY(QPoint offset READ offset WRITE setOffset)
    Q_PROPERTY(QPoint textureOffset READ textureOffset WRITE setTextureOffset)
    Q_PROPERTY(QSize size READ size WRITE setSize)
#endif
public:
    SpritePart(SpriteFrame* root);
    SpritePart(SpriteFrame* root, const std::string& name, bool hasCollision = false);
    virtual ~SpritePart();


#ifndef LIBZELDA_USE_QT
    void setName(const std::string& name);
    std::string name() const;
#else
    void setName(const QString& name);
    QString name() const;
#endif

    void setCollision(bool col);
    bool hasCollision() const;

    /*!
     * \brief setOffset
     * \param x
     * \param y
     */
    void setOffset(float x, float y);

    /*!
     * \brief setOffset
     * \param offset
     */
#ifndef LIBZELDA_USE_QT
    void setOffset(const Vector2Df& offset);
#else
    void setOffset(const QPoint& offset);
#endif

    /*!
     * \brief offset
     * \return
     */
#ifndef LIBZELDA_USE_QT
    Vector2Df offset() const;
#else
    QPoint offset() const;
#endif

    /*!
     * \brief setTextureOffset
     * \param x
     * \param y
     */
    void setTextureOffset(float x, float y);

    /*!
     * \brief setTextureOffset
     * \param texOff
     */
#ifndef LIBZELDA_USE_QT
    void setTextureOffset(const Vector2Df& offset);
#else
    void setTextureOffset(const QPoint& offset);
#endif

    /*!
     * \brief textureOffset
     * \return
     */
#ifndef LIBZELDA_USE_QT
    Vector2Df textureOffset() const;
#else
    QPoint textureOffset() const;
#endif

    /*!
     * \brief setSize
     * \param width
     * \param height
     */
    void setSize(Uint32 width, Uint32 height);

    /*!
     * \brief setSize
     * \param size
     */
#ifndef LIBZELDA_USE_QT
    void setSize(const Vector2Di& size);
#else
    void setSize(const QSize& size);
#endif

    /*!
     * \brief size
     * \return
     */
#ifndef LIBZELDA_USE_QT
    Vector2Di size() const;
#else
    QSize size() const;
#endif

    /*!
     * \brief setFlippedHorizontally
     * \param val
     */
    void setFlippedHorizontally(const bool val);

    /*!
     * \brief flippedHorizontally
     * \return
     */
    bool flippedHorizontally() const;

    /*!
     * \brief setFlippedVertically
     * \param val
     */
    void setFlippedVertically(const bool val);

    /*!
     * \brief flippedVertically
     * \return
     */
    bool flippedVertically() const;

    void setRoot(SpriteFrame* root);
    SpriteFrame* root() const;

#ifdef LIBZELDA_USE_QT
signals:
    void nameChanged(QString);
    void orientationChanged(bool,bool);
    void offsetChanged(QPoint);
    void textureOffsetChanged(QPoint);
    void sizeChanged(QSize);
    void collisionChanged(bool);
#endif

private:
    SpriteFrame*  m_root;
#ifndef LIBZELDA_USE_QT
    std::string   m_name;
#else
    QString       m_name;
#endif
    bool          m_hasCollision;
#ifndef LIBZELDA_USE_QT
    Vector2Df     m_offset;
    Vector2Df     m_textureOffset;
    Vector2Di     m_size;
#else
    QPoint        m_offset;
    QPoint        m_textureOffset;
    QSize         m_size;
#endif
    bool          m_flippedH;
    bool          m_flippedV;
    Uint32        m_frameIndex;
};

}
}
#ifdef LIBZELDA_USE_QT
Q_DECLARE_METATYPE(zelda::Sakura::SpritePart*)
#endif

#endif // SSPRITEPART_HPP
