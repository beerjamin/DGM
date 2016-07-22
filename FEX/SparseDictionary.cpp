#include "SparseDictionary.h"
#include "macroses.h"

namespace DirectGraphicalModels { namespace fex
{
	// J(D, W) = ||W x D - X||^{2}_{2} + \lambda ||W||_1 + \gamma ||D||^{2}_{2}
	void CSparseDictionary::train(const Mat &X, word nWords, dword batch, unsigned int nIt)
	{
		const dword		nSamples	= X.rows;
		const int		sampleLen	= X.cols;

<<<<<<< HEAD
		const float	lambda		= 5e-5f;		// 5e-5;  // L1-regularisation parameter (on features)
		const float	epsilon		= 1e-5f;		// 1e-5;  // L1-regularisation epsilon |x| ~ sqrt(x^2 + epsilon)
		const float	gamma		= 1e-2f;		// 1e-2;  // L2-regularisation parameter (on basis)
=======
		const double	lambda		= 5e-5;		// 5e-5;  // L1-regularisation parameter (on features)
		const double	epsilon		= 1e-5;		// 1e-5;  // L1-regularisation epsilon |x| ~ sqrt(x^2 + epsilon)
		const double	gamma		= 1e-2;		// 1e-2;  // L2-regularisation parameter (on basis)
>>>>>>> 0d589ec2f5d0ed3d74673219e5e86d657f59305f
		
		DGM_ASSERT_MSG(batch <= nSamples, "The batch number %d exceeds the length of the training data %d", batch, nSamples);

		// 1. Initialize dictionary D randomly
		if (!m_D.empty()) m_D.release();
<<<<<<< HEAD
		m_D = Mat(nWords, sampleLen, CV_32FC1);
=======
		m_D = Mat(nWords, sampleLen, CV_64FC1);
>>>>>>> 0d589ec2f5d0ed3d74673219e5e86d657f59305f

		RNG rng;
		rng.fill(m_D, RNG::NORMAL, 0, 0.3);

		Mat		W;	// Weights matrix (Size: nStamples x nWords)
<<<<<<< HEAD
		float	cost;
=======
		double	cost;
>>>>>>> 0d589ec2f5d0ed3d74673219e5e86d657f59305f

		// 2. Repeat until convergence
		for (unsigned int i = 0; i < nIt; i++) {
			printf("--- It: %d ---\n", i);

			// 2.1 Select a random mini-batch of 2000 patches
			dword rndRow = ((dword) rand() * (RAND_MAX + 1) + (dword) rand()) % (nSamples - batch);
			Mat _X = X(cvRect(0, rndRow, sampleLen, batch));

			// 2.2 Initialize W
			gemm(m_D, _X, 1.0, Mat(), 0.0, W, GEMM_2_T);				// W = (D x _X^T)^T;
			W = W.t();
			for (word w = 0; w < W.cols; w++)
				W.col(w) /= norm(m_D.row(w), NORM_L2);
			
			printf("Cost: ");
			cost = calculateCost(_X, m_D, W, lambda, epsilon, gamma);
			printf("%f -> ", cost);
			
			// 2.3. Find the W, that minimizes E(D, W) for the D found in the previos step
			// J(W) = ||W x D - X||^{2}_{2} + \lambda||W||_1
			calculate_W(_X, m_D, W, lambda, epsilon, 800);
			cost = calculateCost(_X, m_D, W, lambda, epsilon, gamma);
			printf("%f -> ", cost);


			// 2.4 Solve for the D that minimizes E(D, W) for the W found in the previous step
			// J(D) = ||W x D - X||^{2}_{2} + \gamma||D||^{2}_{2}
			calculate_D(_X, m_D, W, gamma, 800);
			cost = calculateCost(_X, m_D, W, lambda, epsilon, gamma);
			printf("%f\n", cost);

			// 2.5 Saving intermediate dictionary
			std::string str = "D:\\Dictionaries\\dict_";
			str += std::to_string(i / 5);
			str += ".txt";
			if (i % 5 == 0) save(str);
		} // i

	}

	void CSparseDictionary::save(const std::string &fileName) const
	{
		FILE *pFile = fopen(fileName.c_str(), "wb");
		fwrite(&m_D.rows, sizeof(int), 1, pFile);			// nFeatures
		fwrite(&m_D.cols, sizeof(int), 1, pFile);			// nWords
<<<<<<< HEAD
		fwrite(m_D.data, sizeof(float), m_D.rows * m_D.cols, pFile);
=======
		fwrite(m_D.data, sizeof(double), m_D.rows * m_D.cols, pFile);
>>>>>>> 0d589ec2f5d0ed3d74673219e5e86d657f59305f
		fclose(pFile);
	}

	void CSparseDictionary::load(const std::string &fileName)
	{
		int sampleLen;
		int nWords;

		FILE *pFile = fopen(fileName.c_str(), "rb");
		fread(&sampleLen, sizeof(int), 1, pFile);
		fread(&nWords, sizeof(int), 1, pFile);

		if (!m_D.empty()) m_D.release();
<<<<<<< HEAD
		m_D = Mat(sampleLen, nWords, CV_32FC1);

		fread(m_D.data, sizeof(float), sampleLen * nWords, pFile);
=======
		m_D = Mat(sampleLen, nWords, CV_64FC1);

		fread(m_D.data, sizeof(double), sampleLen * nWords, pFile);
>>>>>>> 0d589ec2f5d0ed3d74673219e5e86d657f59305f

		fclose(pFile);
	}

	Mat CSparseDictionary::decode(const Mat &X, CvSize imgSize) const
	{
		DGM_ASSERT_MSG(!m_D.empty(), "The dictionary must me trained or loaded before using this function");

		const int		blockSize = static_cast<int>(sqrt(m_D.rows));
<<<<<<< HEAD
		const float	lambda		= 5e-5f;		// 5e-5;  // L1-regularisation parameter (on features)
		const float	epsilon		= 1e-5f;		// 1e-5;  // L1-regularisation epsilon |x| ~ sqrt(x^2 + epsilon)
=======
		const double	lambda		= 5e-5;		// 5e-5;  // L1-regularisation parameter (on features)
		const double	epsilon		= 1e-5;		// 1e-5;  // L1-regularisation epsilon |x| ~ sqrt(x^2 + epsilon)
>>>>>>> 0d589ec2f5d0ed3d74673219e5e86d657f59305f

		Mat res(imgSize, CV_32FC1, cvScalar(0));
		Mat cover(imgSize, CV_32FC1, cvScalar(0));

		// for (int s = 0; s < nSamples; s++) {
#ifdef USE_PPL
		concurrency::parallel_for(0, imgSize.height - blockSize + 1, blockSize, [&](int y) {
#else
		for (int y = 0; y < imgSize.height - blockSize + 1; y += blockSize) {
#endif
			for (int x = 0; x < imgSize.width - blockSize + 1; x += blockSize) {

				int s = y * (imgSize.width - blockSize + 1) + x;				// sample index

				Mat sample = X.row(s);											// sample
				Mat W;
				gemm(m_D, sample, 1.0, Mat(), 0.0, W, GEMM_2_T);				// W = (D x sample^T)^T
				W = W.t();
				for (word w = 0; w < W.cols; w++)
					W.col(w) /= norm(m_D.row(w), NORM_L2);

				calculate_W(sample, m_D, W, lambda, epsilon, 800);

				Mat tmp;
				gemm(W, m_D, 1.0, Mat(), 0.0, tmp);								// tmp = W x D
				tmp = tmp.reshape(0, blockSize);

				res(cvRect(x, y, blockSize, blockSize)) += tmp;
				cover(cvRect(x, y, blockSize, blockSize)) += 1.0;
			}
		}
#ifdef USE_PPL
		);
#endif
		res /= cover;
		return res;
		}

	// =================================================================================== static

	float getMean(const Mat &m)
	{
		float sum = 0.0;
		for (int i = 0; i < m.cols; i++)
<<<<<<< HEAD
			sum += m.at<float>(0, i);
=======
			sum += m.at<double>(0, i);
>>>>>>> 0d589ec2f5d0ed3d74673219e5e86d657f59305f
		return sum / m.cols;
	}

	float getVariance(const Mat &m)
	{
		float mean = getMean(m);
		float temp = 0.0;
		for (int i = 0; i < m.cols; i++) {
<<<<<<< HEAD
			float val = m.at<float>(0, i);
=======
			double val = m.at<double>(0, i);
>>>>>>> 0d589ec2f5d0ed3d74673219e5e86d657f59305f
			temp += (mean - val) * (mean - val);
		}
		return temp / m.cols;
	}

	Mat CSparseDictionary::img2data(const Mat &img, int blockSize, float varianceThreshold)
	{
		DGM_IF_WARNING(blockSize % 2 == 0, "The block size is even");

		// Converting to one channel image
		Mat I;
		if (img.channels() != 1) cvtColor(img, I, CV_RGB2GRAY);
		else img.copyTo(I);

		// Converting to floating - point normalized data
<<<<<<< HEAD
		I.convertTo(I, CV_32FC1, 1.0 / 255);
=======
		I.convertTo(I, CV_64FC1, 1.0 / 255);
>>>>>>> 0d589ec2f5d0ed3d74673219e5e86d657f59305f

		const int	dataHeight  = img.rows - blockSize + 1;
		const int	dataWidth	= img.cols - blockSize + 1;

		Mat res;
		Mat sample;

		for (int y = 0; y < dataHeight; y++)
			for (int x = 0; x < dataWidth; x++) {
				sample = I(cvRect(x, y, blockSize, blockSize)).clone().reshape(0, 1);			// sample as a row-vector
				
<<<<<<< HEAD
				float variance = getVariance(sample);
=======
				double variance = getVariance(sample);
>>>>>>> 0d589ec2f5d0ed3d74673219e5e86d657f59305f
				//printf("variance = %f\n", variance);

				if (variance >= varianceThreshold) 
					res.push_back(sample);
			} // x
		return res;
	}

	Mat CSparseDictionary::data2img(const Mat &X, CvSize imgSize)
	{
<<<<<<< HEAD
		Mat res(imgSize, CV_32FC1, cvScalar(0));
		Mat cover(imgSize, CV_32FC1, cvScalar(0));
=======
		Mat res(imgSize, CV_64FC1, cvScalar(0));
		Mat cover(imgSize, CV_64FC1, cvScalar(0));
>>>>>>> 0d589ec2f5d0ed3d74673219e5e86d657f59305f

		const int	blockSize	= static_cast<int>(sqrt(X.cols));
		const int	dataWidth	= res.cols - blockSize + 1;
		const int	dataHeight	= res.rows - blockSize + 1;

		for (int s = 0; s < X.rows; s++) {
			Mat sample = X.row(s);															// smple as a row-vector
			sample = sample.reshape(0, blockSize);												// square sample - data patch

			int y = s / dataWidth;
			int x = s % dataWidth;

			res(cvRect(x, y, blockSize, blockSize)) += sample;
			cover(cvRect(x, y, blockSize, blockSize)) += 1.0;
		}
		res /= cover;
		
		res.convertTo(res, CV_8UC1, 255);
		return res;
	}

	Mat CSparseDictionary::shuffleRows(const Mat &X)
	{
		std::vector<int> seeds;
		for (int s = 0; s < X.rows; s++) 
			seeds.push_back(s);

		randShuffle(seeds);

		Mat res(X.size(), X.type());
		for (int s = 0; s < X.rows; s++)
			X.row(seeds[s]).copyTo(res.row(s));

		return res;
	}
	
	// =================================================================================== protected

<<<<<<< HEAD
	void my_gemm(const Mat &A, const Mat &B, float alpha, const Mat &C, float beta, Mat &res)
=======
	void my_gemm(const Mat &A, const Mat &B, double alpha, const Mat &C, double beta, Mat &res)
>>>>>>> 0d589ec2f5d0ed3d74673219e5e86d657f59305f
	{
		DGM_ASSERT(A.cols == B.rows);

		const Mat _B = B.t();
<<<<<<< HEAD
		res = Mat(A.rows, B.cols, CV_32FC1);
		concurrency::parallel_for(0, res.rows, 1, [&](int y) {
		//for (int y = 0; y < res.rows; y++) {
			float		 *pRes	= res.ptr<float>(y);
			const float *pA	= A.ptr<float>(y);
			const float *pC    = C.ptr<float>(y);
			for (register int x = 0; x < res.cols; x++) {
				const float *pB = _B.ptr<float>(x);
				float val = 0.0;
=======
		res = Mat(A.rows, B.cols, CV_64FC1);
		concurrency::parallel_for(0, res.rows, 1, [&](int y) {
		//for (int y = 0; y < res.rows; y++) {
			double		 *pRes	= res.ptr<double>(y);
			const double *pA	= A.ptr<double>(y);
			const double *pC    = C.ptr<double>(y);
			for (register int x = 0; x < res.cols; x++) {
				const double *pB = _B.ptr<double>(x);
				double val = 0.0;
>>>>>>> 0d589ec2f5d0ed3d74673219e5e86d657f59305f
				for (register int i = 0; i < A.cols; i++)
					val += pA[i] * pB[i];
				pRes[x] = alpha * val + beta * pC[x];
			} // x

		}); // y

	}


	/// @todo optimize the matrix multiplication
<<<<<<< HEAD
	Mat CSparseDictionary::calculateGradient(grad_type gType, const Mat &X, const Mat &D, const Mat &W, float lambda, float epsilon, float gamma)
=======
	Mat CSparseDictionary::calculateGradient(grad_type gType, const Mat &X, const Mat &D, const Mat &W, double lambda, double epsilon, double gamma)
>>>>>>> 0d589ec2f5d0ed3d74673219e5e86d657f59305f
	{
		const int nSamples = X.rows;
		
		Mat temp; // = (2.0 / nSamples) * (W * D - X);
<<<<<<< HEAD
		my_gemm(W, D, 2.0f / nSamples, X, -2.0f / nSamples, temp);				// temp = 2 * (W x D - X) / nSamples
=======
		my_gemm(W, D, 2.0 / nSamples, X, -2.0 / nSamples, temp);				// temp = 2 * (W x D - X) / nSamples
>>>>>>> 0d589ec2f5d0ed3d74673219e5e86d657f59305f
		Mat gradient;
		Mat sparsityMatrix;

		switch (gType) {
			case GRAD_W:	// 2 * (W x D - X) x D^T / nSamples + lambda * W / sqrt(W^2 + epsilon)
				multiply(W, W, sparsityMatrix);
				sparsityMatrix += epsilon;
				sqrt(sparsityMatrix, sparsityMatrix);												// sparsityMatrix = sqrt(W^2 + epsilon)
				
				my_gemm(temp, D.t(), 1.0, W / sparsityMatrix, lambda, gradient);
				//gradient = temp * D.t();
				//gradient += lambda * (W / sparsityMatrix);							
				break;
			case GRAD_D:	// 2 * W^T x (W x D - X) / nSamples + 2 * gamma * D
				
				my_gemm(W.t(), temp, 1.0, D, 2 * gamma, gradient);
				//gradient = W.t() * temp;										
				//gradient += 2 * gamma * D;
				
				break;
		}

		return gradient;

		// int nsamples = x.n_cols;
		
		// mat sparsityMatrix = sqrt(pow(h, 2.0) + epsilon);
		// hGrad    = 2 * dict.t() * (dict * h - x) / nsamples + lambda * (h / sparsityMatrix);

		// dictGrad = 2 * (dict * h - x) * h.t() / nsamples + 2 * gamma * dict;
	}

	/// @todo Check performance with pow() instead of multiply()
<<<<<<< HEAD
	float CSparseDictionary::calculateCost(const Mat &X, const Mat &D, const Mat &W, float lambda, float epsilon, float gamma)
=======
	double CSparseDictionary::calculateCost(const Mat &X, const Mat &D, const Mat &W, double lambda, double epsilon, double gamma)
>>>>>>> 0d589ec2f5d0ed3d74673219e5e86d657f59305f
	{
		Mat temp;
		
		my_gemm(W, D, 1.0, X, -1.0, temp);					// temp =  W x D - X	
		reduce(temp, temp, 0, CV_REDUCE_AVG);
		multiply(temp, temp, temp);						// temp = (W x D - X)^2
<<<<<<< HEAD
		float J1 = static_cast<float>(sum(temp)[0]);
=======
		double J1 = sum(temp)[0];
>>>>>>> 0d589ec2f5d0ed3d74673219e5e86d657f59305f
		
		multiply(W, W, temp);							// temp = W^2
		temp += epsilon;								// temp = W^2 + epsilon
		sqrt(temp, temp);								// temp = sqrt(W^2 + epsilon)
		reduce(temp, temp, 0, CV_REDUCE_AVG);
<<<<<<< HEAD
		float J2 = lambda * static_cast<float>(sum(temp)[0]);

		multiply(D, D, temp);							// temp = D^2
		float J3 = gamma * static_cast<float>(sum(temp)[0]);

		float cost = J1 + J2 + J3;
=======
		double J2 = lambda * sum(temp)[0];

		multiply(D, D, temp);							// temp = D^2
		double J3 = gamma * sum(temp)[0];

		double cost = J1 + J2 + J3;
>>>>>>> 0d589ec2f5d0ed3d74673219e5e86d657f59305f
		return cost;


		// mat delta = dict * h - x;
<<<<<<< HEAD
		// float J1 = accu(pow(mean(delta, 1), 2.0));
		
		// mat sparsityMatrix = sqrt(pow(h, 2.0) + epsilon);
		// float J2 = lambda * accu(mean(sparsityMatrix, 1));
		
		// float J3 = gamma * accu(pow(dict, 2.0));
		
		// float cost = J1 + J2 + J3;
	}

	// J(W) = ||W x D - X||^{2}_{2} + \lambda||W||_1
	void CSparseDictionary::calculate_W(const Mat &X, const Mat &D, Mat &W, float lambda, float epsilon, unsigned int nIt)
=======
		// double J1 = accu(pow(mean(delta, 1), 2.0));
		
		// mat sparsityMatrix = sqrt(pow(h, 2.0) + epsilon);
		// double J2 = lambda * accu(mean(sparsityMatrix, 1));
		
		// double J3 = gamma * accu(pow(dict, 2.0));
		
		// double cost = J1 + J2 + J3;
	}

	// J(W) = ||W x D - X||^{2}_{2} + \lambda||W||_1
	void CSparseDictionary::calculate_W(const Mat &X, const Mat &D, Mat &W, double lambda, double epsilon, unsigned int nIt)
>>>>>>> 0d589ec2f5d0ed3d74673219e5e86d657f59305f
	{
		// Define the velocity vectors
		Mat gradient;
		Mat incriment(W.size(), W.type(), cvScalar(0));

<<<<<<< HEAD
		const float lrate		= 0.01f;						//Learning rate for weights 
		const float weightcost	= 0.0002f;

		for (unsigned int i = 0; i < nIt; i++) {
			float momentum = (i <= 10) ? 0.5f : 0.9f;
			gradient  = calculateGradient(GRAD_W, X, D, W, lambda, epsilon, 0);
=======
		const double lrate		= 0.01;						//Learning rate for weights 
		const double weightcost = 0.0002;

		for (unsigned int i = 0; i < nIt; i++) {
			double momentum = (i <= 10) ? 0.5 : 0.9;
			gradient  = calculateGradient(GRAD_W, X, D, W, lambda, epsilon, 0.0);
>>>>>>> 0d589ec2f5d0ed3d74673219e5e86d657f59305f
			incriment = momentum * incriment + lrate * (gradient - weightcost * W);
			W -= incriment;
		} // i
	}

	// J(D) = ||W x D - X||^{2}_{2} + \gamma||D||^{2}_{2}
<<<<<<< HEAD
	void CSparseDictionary::calculate_D(const Mat &X, Mat &D, const Mat &W, float gamma, unsigned int nIt)
=======
	void CSparseDictionary::calculate_D(const Mat &X, Mat &D, const Mat &W, double gamma, unsigned int nIt)
>>>>>>> 0d589ec2f5d0ed3d74673219e5e86d657f59305f
	{
		// define the velocity vectors
		Mat gradient;
		Mat incriment(D.size(), D.type(), cvScalar(0));

<<<<<<< HEAD
		const float	lrate		= 0.01f;					//Learning rate for weights 
		const float	weightcost	= 0.0002f;

		for (unsigned int i = 0; i < nIt; i++) {
			float momentum = (i <= 10) ? 0.5f : 0.9f;
			gradient  = calculateGradient(GRAD_D, X, D, W, 0, 0, gamma);
=======
		const double	lrate		= 0.01;					//Learning rate for weights 
		const double	weightcost	= 0.0002;

		for (unsigned int i = 0; i < nIt; i++) {
			double momentum = (i <= 10) ? 0.5 : 0.9;
			gradient  = calculateGradient(GRAD_D, X, D, W, 0.0, 0.0, gamma);
>>>>>>> 0d589ec2f5d0ed3d74673219e5e86d657f59305f
			incriment = momentum * incriment + lrate * (gradient - weightcost * D);
			D -= incriment;
		} // i
	}

} }