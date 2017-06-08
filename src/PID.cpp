#include "PID.h"

#include <stdlib.h>
#include <stdio.h>

#include <limits>


PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp, double Ki, double Kd, int iterations, double dKp, double dKi, double dKd) {
  params[0] = Kp;
  params[1] = Ki;
  params[2] = Kd;
  dparams[0] = dKp;
  dparams[1] = dKi;
  dparams[2] = dKd;
  max_count = iterations;

  d_error = p_error = i_error = 0.0;
  err = 0.0;
  best_err = std::numeric_limits<double>::max();
  count = index = 0;
  up_branch = true;
}

void PID::UpdateError(double cte) {
  d_error = cte - p_error;
  i_error += cte;
  p_error = cte;

  if (max_count) {
    count++;
    if (count > (max_count / 2)) {
      err += cte*cte;

      if (count == max_count) {
        err /= count;
        OptimizeParams();
        printf("PARAM UPDATE: Kp=%f | Ki=%f | Kd=%f | err:%f best:%f\n",
                params[0], params[1], params[2], err, best_err);
        count = 0;
        err = 0.0;
      }
    }
  }
}

double PID::TotalError() {
  return - params[0] * p_error - params[1] * i_error - params[2] * d_error;
}

void PID::OptimizeParams() {

  if (up_branch) {
    if (err < best_err) {
      best_err = err;
      dparams[index] *= 1.1;
      OptimizeNext();
    }
    else {
      params[index] -= 2 * dparams[index];
      up_branch = false;
    }
  }
  else {
    if (err < best_err) {
      best_err = err;
      dparams[index] *= 1.1;
    }
    else {
      params[index] += dparams[index];
      dparams[index] *= 0.9;
    }
    up_branch = true;
    OptimizeNext();
  }
}

void PID::OptimizeNext() {
  params[index] += dparams[index];
  do {
    index = ++index % 3;
  } while (dparams[index] == 0);
}
