FROM ruby:2.6.3

RUN mkdir /src

ENV APP_ROOT /src

WORKDIR $APP_ROOT

# 必要なものをインストール
RUN apt-get update
RUN apt-get install -y apt-utils nodejs default-mysql-client mysql-server
RUN rm -rf /var/lib/apt/lists/*

# ホスト側のGemfileをコピー
ADD Gemfile $APP_ROOT
ADD Gemfile.lock $APP_ROOT

# bundle install
RUN  bundle config --global build.nokogiri --use-system-libraries
RUN  bundle config --global jobs 4
RUN gem install bundler & bundle install

ADD . $APP_ROOT

# docker runした時に起動するコマンドを設定、ポートは3000を設定
EXPOSE  3000
#CMD ["rails", "server", "-b", "0.0.0.0"]