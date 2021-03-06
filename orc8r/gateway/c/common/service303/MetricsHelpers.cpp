/**
 * Copyright 2020 The Magma Authors.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "MetricsHelpers.h"

#include "MetricsSingleton.h"

namespace magma {
namespace service303 {

void increment_counter(
    const char* name, double increment, size_t n_labels, ...) {
  va_list ap;
  va_start(ap, n_labels);
  MetricsSingleton::Instance().IncrementCounter(name, increment, n_labels, ap);
  va_end(ap);
}

void increment_gauge(const char* name, double increment, size_t n_labels, ...) {
  va_list ap;
  va_start(ap, n_labels);
  MetricsSingleton::Instance().IncrementGauge(name, increment, n_labels, ap);
  va_end(ap);
}

void decrement_gauge(const char* name, double decrement, size_t n_labels, ...) {
  va_list ap;
  va_start(ap, n_labels);
  MetricsSingleton::Instance().DecrementGauge(name, decrement, n_labels, ap);
  va_end(ap);
}

void set_gauge(const char* name, double value, size_t n_labels, ...) {
  va_list ap;
  va_start(ap, n_labels);
  MetricsSingleton::Instance().SetGauge(name, value, n_labels, ap);
  va_end(ap);
}

void observe_histogram(
    const char* name, double observation, size_t n_labels, ...) {
  va_list ap;
  va_start(ap, n_labels);
  MetricsSingleton::Instance().ObserveHistogram(
      name, observation, n_labels, ap);
  va_end(ap);
}

}  // namespace service303
}  // namespace magma