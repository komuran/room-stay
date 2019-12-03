EXEC=docker-compose exec

all: docker/start migrate/init migrate/up migrate/seed run ## docker start & migrate all & rails run

bundle/install: ## bundle install
	$(EXEC) app bundle install

run: ## rais run
	$(EXEC) app rails s -b 0.0.0.0

migrate/init: ## migrate database
	$(EXEC) app rails db:create

migrate/up: ## migrate up
	$(EXEC) app rails db:migrate

migrate/seed: ## migrate seed
	$(EXEC) app rails db:seed

migrate/status: ## migrate status
	$(EXEC) app rails db:migrate:status

migrate/down: ## migrate rollback
	$(EXEC) app rails db:rollback

docker/build: ## docker build
	docker-compose build

docker/start: ## docker start
	docker-compose up -d

docker/logs: ## docker logs
	docker-compose logs

docker/stop: ## docker stop
	docker-compose stop

docker/clean: ## docker clean
	docker-compose rm

app/bash: ## app(rails) container bash
	$(EXEC) app bash

db/bash: ## MySQL container bash
	$(EXEC) db bash

help: ## Display this help screen
	@grep -E '^[a-zA-Z/_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-20s\033[0m %s\n", $$1, $$2}'
