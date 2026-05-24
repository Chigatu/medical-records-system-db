#pragma once
#include <memory>
#include "InMemoryUserRepository.h"
#include "InMemoryPatientRepository.h"
#include "InMemoryMedicalRecordRepository.h"
#include "../services/JWTService.h"

namespace medical_api {

class RepositoryFactory {
public:
    static std::shared_ptr<medical::infrastructure::InMemoryUserRepository> GetUserRepo() {
        static auto repo = std::make_shared<medical::infrastructure::InMemoryUserRepository>();
        return repo;
    }
    
    static std::shared_ptr<medical::infrastructure::InMemoryPatientRepository> GetPatientRepo() {
        static auto repo = std::make_shared<medical::infrastructure::InMemoryPatientRepository>();
        return repo;
    }
    
    static std::shared_ptr<medical::infrastructure::InMemoryMedicalRecordRepository> GetMedicalRecordRepo() {
        static auto repo = std::make_shared<medical::infrastructure::InMemoryMedicalRecordRepository>();
        return repo;
    }
    
    static std::shared_ptr<medical::infrastructure::JWTService> GetJwtService() {
        static auto jwt = std::make_shared<medical::infrastructure::JWTService>();
        return jwt;
    }
};

}  // namespace medical_api
