# Medical Records Management System (с базой данных)
REST API сервис для управления медицинскими записями с PostgreSQL (Вариант 20, ДЗ №3).
## Функциональность
- Регистрация и аутентификация пользователей (JWT)
- Поиск пользователей по маске имени
- Регистрация пациентов
- Поиск пациентов по ФИО
- Создание медицинских записей
- Получение истории записей пациента
- Получение записи по уникальному коду
## Технологии
- C++20 + **userver** (асинхронный фреймворк Яндекса)
- **PostgreSQL 15** — реляционная база данных
- JWT аутентификация
- Docker (docker-compose)
## Схема базы данных
### Таблица `users` — Пользователи (врачи)
| Колонка | Тип | Ограничения | Описание |
|---------|-----|-------------|----------|
| id | SERIAL | PRIMARY KEY | Уникальный идентификатор |
| login | VARCHAR(50) | NOT NULL, UNIQUE | Логин для входа |
| email | VARCHAR(100) | NOT NULL, CHECK | Email пользователя |
| first_name | VARCHAR(100) | NOT NULL | Имя |
| last_name | VARCHAR(100) | NOT NULL | Фамилия |
| patronymic | VARCHAR(100) | | Отчество |
| password_hash | VARCHAR(255) | NOT NULL | Хеш пароля |
| is_active | BOOLEAN | DEFAULT TRUE | Активен ли пользователь |
| created_at | TIMESTAMP | DEFAULT NOW() | Дата создания |
### Таблица `patients` — Пациенты
| Колонка | Тип | Ограничения | Описание |
|---------|-----|-------------|----------|
| id | SERIAL | PRIMARY KEY | Уникальный идентификатор |
| user_id | INTEGER | FOREIGN KEY → users(id) | Связь с пользователем |
| first_name | VARCHAR(100) | NOT NULL | Имя |
| last_name | VARCHAR(100) | NOT NULL | Фамилия |
| patronymic | VARCHAR(100) | | Отчество |
| phone | VARCHAR(20) | | Телефон |
| address | TEXT | | Адрес |
| birth_date | DATE | | Дата рождения |
| snils | VARCHAR(20) | UNIQUE | Страховой номер |
| policy_number | VARCHAR(20) | | Номер полиса |
| created_at | TIMESTAMP | DEFAULT NOW() | Дата создания |
### Таблица `medical_records` — Медицинские записи
| Колонка | Тип | Ограничения | Описание |
|---------|-----|-------------|----------|
| id | SERIAL | PRIMARY KEY | Уникальный идентификатор |
| code | VARCHAR(30) | NOT NULL, UNIQUE | Код записи (MED-YYYYMMDD-XXXXX) |
| patient_id | INTEGER | NOT NULL, FK → patients(id) | Пациент |
| doctor_id | INTEGER | FK → users(id) | Врач |
| diagnosis_code | VARCHAR(10) | | Код диагноза (МКБ) |
| diagnosis_description | TEXT | | Описание диагноза |
| complaints | TEXT | | Жалобы |
| recommendations | TEXT | | Рекомендации |
| created_at | TIMESTAMP | DEFAULT NOW() | Дата создания |
## Индексы
- `idx_patients_user_id` — для JOIN patients с users
- `idx_records_patient_id` — для JOIN medical_records с patients
- `idx_records_doctor_id` — для JOIN medical_records с users
- `idx_patients_full_name` — для поиска пациентов по ФИО
- `idx_patients_snils` — для быстрой проверки уникальности СНИЛС
- `idx_users_login` — для быстрого поиска пользователя по логину
- `idx_records_code` — для поиска записи по коду
- `idx_patients_name_search` — GIN индекс для полнотекстового поиска
## Быстрый старт (Docker)
1. Установите Docker Desktop
2. Склонируйте репозиторий:
```
git clone https://github.com/Chigatu/medical-records-system-db.git
cd medical-records-system-db
```
3. Запустите сервисы (API + PostgreSQL):
```
docker-compose up --build
```
4. Проверьте:
```
curl http://localhost:8080/health
```
## Структура проекта
```
medical-records-system-db/
├── src/                    # Исходный код API
│   ├── handlers/           # HTTP обработчики
│   ├── models/             # Доменные модели
│   ├── repositories/       # Репозитории (in-memory → PostgreSQL)
│   ├── services/           # Бизнес-логика
│   └── main.cpp            # Точка входа
├── configs/                # Конфигурация userver
├── database/               # База данных
│   ├── schema.sql          # Схема БД (CREATE TABLE, индексы)
│   ├── data.sql            # Тестовые данные (12 врачей, 12 пациентов, 12 записей)
│   ├── queries.sql         # SQL запросы для всех операций API
│   └── optimization.md     # Оптимизация запросов с EXPLAIN
├── Dockerfile              # Docker образ API
├── docker-compose.yaml     # Docker Compose (API + PostgreSQL)
└── README.md               # Документация
```
## API Endpoints
| Метод | URL | Описание | Требуется JWT |
|-------|-----|----------|---------------|
| GET | /health | Проверка работоспособности | Нет |
| POST | /api/auth/register | Регистрация пользователя | Нет |
| POST | /api/auth/login | Вход в систему | Нет |
| POST | /api/patients | Создание пациента | Да |
| GET | /api/patients/search?fullName={name} | Поиск пациентов по ФИО | Нет |
| POST | /api/medical-records | Создание медицинской записи | Да |
| GET | /api/medical-records/patient/{id} | Получение истории записей пациента | Да |
## Тестирование
```
./test_api_userver.sh
```
