//
#ifndef SRC_COMPONENTS_METADATA_HPP_
#define SRC_COMPONENTS_METADATA_HPP_

#include <vector>
namespace RType {
namespace Components {
class Metadata {
    public:
        enum MetadataList {
            MetadataListNA,
            MetadataListBoss,
            MetadataListEscort
        };
        Metadata(void) = default;
        ~Metadata(void) = default;
        void add(MetadataList meta) {
            this->_metadataVector.push_back(meta);
        }
        bool check(MetadataList meta) {
            for (auto it : this->_metadataVector) {
                if (it == meta) {
                    return true;
                }
            }
            return false;
        }

    private:
        std::vector<MetadataList> _metadataVector;
};  // class Metadata
}  // namespace Components
}  // namespace RType
#endif  // SRC_COMPONENTS_METADATA_HPP_
