#ifndef PID_H
#define PID_H

class PID {
public:
  /*
  * Errors
  */
  double p_error;
  double i_error;
  double d_error;
  /*
  * Coefficients
  */
  double params[3];

  /*
  * Constructor
  */
  PID();

  /*
  * Destructor.
  */
  virtual ~PID();

  /*
  * Initialize PID.
  */
  void Init(double Kp, double Ki, double Kd, int twiddle_count = 0,
                                             double dKp = 0.0,
                                             double dKi = 0.0,
                                             double dKd = 0.0);

  /*
  * Update the PID error variables given cross track error.
  */
  void UpdateError(double cte);

  /*
  * Calculate the total PID error.
  */
  double TotalError();

private:

  void OptimizeParams();
  void OptimizeNext();

  double dparams[3];
  double err;
  double best_err;
  int count;
  int index;
  int max_count;
  bool up_branch;
};

#endif /* PID_H */
