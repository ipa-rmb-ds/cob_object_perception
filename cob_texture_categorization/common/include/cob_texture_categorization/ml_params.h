/*
 * ml_params.h
 *
 *  Created on: 08.05.2015
 *      Author: Richard Bormann
 */

#ifndef ML_PARAMS_H_
#define ML_PARAMS_H_

#include <opencv2/opencv.hpp>
#include <vector>
#include <sstream>
#include <iostream>

class MLParams
{
public:
	enum ClassificationMethod {SVM, NEURAL_NETWORK, KNN, DECISION_TREE, RANDOM_TREES, NAIVE_BAYES, BOOSTING};

	// ML method
	ClassificationMethod classification_method_;

	// general termination criteria
	cv::TermCriteria term_criteria_;

	// SVM specific
#if CV_MAJOR_VERSION == 2
	cv::SVMParams svm_params_;
#endif
	int svm_params_svm_type_;
	int svm_params_kernel_type_;
	double svm_params_degree_;
	double svm_params_gamma_;
	double svm_params_coef0_;
	double svm_params_C_;
	double svm_params_nu_;
	double svm_params_p_;

	// Neural Network specific
#if CV_MAJOR_VERSION == 2
	CvANN_MLP_TrainParams nn_params_;
#endif
	int nn_params_train_method_;
	double nn_params_bp_dw_scale_;
	double nn_params_bp_moment_scale_;
	std::vector<int> nn_hidden_layers_;		// sizes of the hidden layers of the neural network classifier (i.e. with only one entry 10, it will yield a neural network with one hidden layer with 10 neurons)
	int nn_activation_function_;
	double nn_activation_function_param1_;
	double nn_activation_function_param2_;

	// constructor for usage with SVM
	MLParams(ClassificationMethod classification_method, int term_criteria_type, int term_criteria_max_iter, double term_criteria_epsilon,
			int svm_params_svm_type, int svm_params_kernel_type, double svm_params_degree, double svm_params_gamma, double svm_params_coef0,
			double svm_params_C, double svm_params_nu, double svm_params_p)
	{
		classification_method_ = classification_method;
		term_criteria_ = cv::TermCriteria(term_criteria_type, term_criteria_max_iter, term_criteria_epsilon);
		svm_params_svm_type_ = svm_params_svm_type;
		svm_params_kernel_type_ = svm_params_kernel_type;
		svm_params_degree_ = svm_params_degree;
		svm_params_gamma_ = svm_params_gamma;
		svm_params_coef0_ = svm_params_coef0;
		svm_params_C_ = svm_params_C;
		svm_params_nu_ = svm_params_nu;
		svm_params_p_ = svm_params_p;
#if CV_MAJOR_VERSION == 2
		svm_params_ = cv::SVMParams(svm_params_svm_type, svm_params_kernel_type, svm_params_degree, svm_params_gamma, svm_params_coef0, svm_params_C, svm_params_nu, svm_params_p, 0, term_criteria_);
#endif

		nn_activation_function_ = 0; nn_activation_function_param1_=0; nn_activation_function_param2_=0;
	}

	// constructor for usage with Neural Networks
	MLParams(ClassificationMethod classification_method, int term_criteria_type, int term_criteria_max_iter, double term_criteria_epsilon,
			int nn_params_train_method, double nn_params_bp_dw_scale, double nn_params_bp_moment_scale, const std::vector<int>& nn_hidden_layers,
			int nn_activation_function, double nn_activation_function_param1, double nn_activation_function_param2)
	{
		classification_method_ = classification_method;
		term_criteria_ = cv::TermCriteria(term_criteria_type, term_criteria_max_iter, term_criteria_epsilon);
		nn_params_train_method_ = nn_params_train_method;
		nn_params_bp_dw_scale_ = nn_params_bp_dw_scale;
		nn_params_bp_moment_scale_ = nn_params_bp_moment_scale;
#if CV_MAJOR_VERSION == 2
		nn_params_.train_method = nn_params_train_method;
		nn_params_.bp_dw_scale = nn_params_bp_dw_scale;
		nn_params_.bp_moment_scale = nn_params_bp_moment_scale;
		nn_params_.term_crit = term_criteria_;
#endif
		nn_hidden_layers_ = nn_hidden_layers;
		nn_activation_function_ = nn_activation_function;
		nn_activation_function_param1_=nn_activation_function_param1;
		nn_activation_function_param2_=nn_activation_function_param2;
	}

	std::string configurationToString() const
	{
		std::stringstream ss;

		if (classification_method_ == SVM)
		{
			ss << "Classification method: SVM\n";
			ss << "CvSVMParams:"
					<< "\n\t.svm_type: " << svm_params_svm_type_
					<< "\n\t.kernel_type: " << svm_params_kernel_type_
					<< "\n\t.degree: " << svm_params_degree_
					<< "\n\t.gamma: " << svm_params_gamma_
					<< "\n\t.coef0: " << svm_params_coef0_
					<< "\n\t.C: " << svm_params_C_
					<< "\n\t.nu: " << svm_params_nu_
					<< "\n\t.p: " << svm_params_p_
					<< "\n";
		}
		else if (classification_method_ == NEURAL_NETWORK)
		{
			ss << "Classification method: Neural Network\n";
			ss << "CvANN_MLP_TrainParams:"
					<< "\n\t.train_method: " << nn_params_train_method_
					<< "\n\t.bp_dw_scale: " << nn_params_bp_dw_scale_
					<< "\n\t.bp_moment_scale: " << nn_params_bp_moment_scale_
#if CV_MAJOR_VERSION == 2
					<< "\n\t.rp_dw0: " << nn_params_.rp_dw0
					<< "\n\t.rp_dw_plus: " << nn_params_.rp_dw_plus
					<< "\n\t.rp_dw_minus: " << nn_params_.rp_dw_minus
					<< "\n\t.rp_dw_min: " << nn_params_.rp_dw_min
					<< "\n\t.rp_dw_max: " << nn_params_.rp_dw_max
#endif
					<< "\n";
			ss << "Hidden layers configuration:  ";
			for (size_t i=0; i<nn_hidden_layers_.size(); ++i)
				ss << nn_hidden_layers_[i] << "  ";
			ss << "\nActivation function: " << nn_activation_function_ << "\tparam1: " << nn_activation_function_param1_ << "\tparam2: " << nn_activation_function_param2_ << "\n";
		}
		else
		{
			std::cout << "Warning: MLParams::configurationToString: auto-configuration and print out of selected classification method " << classification_method_ << " not yet implemented." << std::endl;
		}

		ss << "\nCvTermCriteria:"
				<< "\n\t.type: " << term_criteria_.type
				<< "\n\t.maxCount: " << term_criteria_.maxCount
				<< "\n\t.epsilon: " << term_criteria_.epsilon
				<< "\n";

		return ss.str();
	}
};

class CrossValidationParams
{
public:
	enum CrossValidationMode {LEAVE_OUT_ONE_OBJECT_PER_CLASS = 0, LEAVE_OUT_ONE_CLASS = 1, DTD_SPLITS = 2};

	// cross validation paramters
	CrossValidationMode cross_validation_mode_;
	unsigned int folds_;
	unsigned int number_classes_;		// number of different labels

	// sets of machine learning parameters (for testing one or multiple ML configurations - cross validation will be run as many times as MLParams provided)
	std::vector<MLParams> ml_configurations_;

	CrossValidationParams(CrossValidationMode cross_validation_mode, unsigned int folds, unsigned int number_classes)
	{
		cross_validation_mode_ = cross_validation_mode;
		folds_ = folds;
		number_classes_ = number_classes;
	}

	std::string configurationToString() const
	{
		std::stringstream ss;

		if (cross_validation_mode_ == LEAVE_OUT_ONE_OBJECT_PER_CLASS)
			ss << folds_ << "-fold leave out one object per class cross-validation:\n---------------------------------------------------------\n";
		else if (cross_validation_mode_ == LEAVE_OUT_ONE_CLASS)
			ss << folds_ << "-fold leave out one class cross-validation:\n----------------------------------------------\n";
		else if (cross_validation_mode_ == DTD_SPLITS)
			ss << folds_ << "-fold cross-validation with official DTD sets:\n-------------------------------------------------\n";
		else
			std::cout << "Warning: CrossValidationParams::configurationToString: unsupported cross-validation method " << cross_validation_mode_ << "." << std::endl;

		return ss.str();
	}
};

#endif // ML_PARAMS_H_
