#pragma once

class Qimage;

#include <SeExpression.h>
#include <QThread>
#include <QMutex>

QImage* MakeImage(const std::string& exprStr, int width, int height, std::vector<SeExpression::Error>& errors);

class Imager
{
public:
	Imager(int width, int height);
	virtual ~Imager();
	void UpdateExpression(const std::string& exprStr);
	QImage* GetImage();
	QPixmap* GetPixmap();
	std::vector<SeExpression::Error> GetErrors() const;
private:
	int m_width;
	int m_height;

	class RenderThread : public QThread
	{
	public:
		RenderThread(const std::string& expression, int width, int height, QImage* displayImage, std::vector<SeExpression::Error>& errors);
		void run();
	private:

		void UpdateDisplayImage();
		const std::string& m_expression;
		std::string m_currentExpression;
		std::string m_lastExpression;
		std::vector<SeExpression::Error>& m_errors;
		std::vector<SeExpression::Error> m_currentErrors;
		QImage* m_renderImage;
		QImage* m_displayImage;

		int m_width;
		int m_height;
	};

	QImage* m_displayImage;
	RenderThread* m_renderThread;
	std::string m_expression;
	std::vector<SeExpression::Error> m_errors;
};
