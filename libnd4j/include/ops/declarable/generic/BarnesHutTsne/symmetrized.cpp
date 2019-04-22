/*******************************************************************************
 * Copyright (c) 2015-2018 Skymind, Inc.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Apache License, Version 2.0 which is available at
 * https://www.apache.org/licenses/LICENSE-2.0.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 * SPDX-License-Identifier: Apache-2.0
 ******************************************************************************/

//
// @author George A. Shulinok <sgazeos@gmail.com>, created on 4/18/2019.
//

#include <op_boilerplate.h>
#if NOT_EXCLUDED(OP_barnes_symmetrized)

#include <ops/declarable/CustomOperations.h>
#include <ops/declarable/helpers/BarnesHutTsne.h>

namespace nd4j {
namespace ops  {
		NDArray* rowCountsPtr = nullptr;

		CUSTOM_OP_IMPL(barnes_symmetrized, 3, 1, false, 0, 0) {
    		auto rowP  = INPUT_VARIABLE(0);
            auto colP  = INPUT_VARIABLE(1);
            auto valP  = INPUT_VARIABLE(2);

    		auto output = OUTPUT_VARIABLE(0);
            if (rowCountsPtr) {
                helpers::barnes_symmetrize(rowP, colP, valP, output, rowCountsPtr);
                delete rowCountsPtr;
                return Status::OK();
            }
            return Status::THROW("barnes_symmetrized: Cannot loop due wrong input data.");
		}

		DECLARE_TYPES(barnes_symmetrized) {
			getOpDescriptor()
				->setAllowedInputTypes(nd4j::DataType::ANY)
				->setSameMode(true);
		}

		DECLARE_SHAPE_FN(barnes_symmetrized) {
    		auto valPShapeInfo = inputShape->at(2);
            Nd4jLong* outShapeInfo;
            auto rowP  = INPUT_VARIABLE(0);
            auto colP  = INPUT_VARIABLE(1);
            NDArray* rowCounts = rowP->dup();
            Nd4jLong len = helpers::barnes_row_count(rowP, colP, *rowCounts);
            if (len <= 0) throw std::runtime_error("barnes_symmetrized: Cannot allocate shape due non-positive len.");
            rowCountsPtr = rowCounts;
            outShapeInfo = ShapeBuilders::createVectorShapeInfo(ArrayOptions::dataType(valPShapeInfo), len, block.workspace());

    		return SHAPELIST(outShapeInfo);
		}

}
}

#endif