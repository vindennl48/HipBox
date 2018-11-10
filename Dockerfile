FROM starefossen/ruby-node:2-10

RUN apt-get update && apt-get -y upgrade && apt-get install -y build-essential
RUN gem install rails -v 5.2.0

WORKDIR /app
ADD Gemfile Gemfile.lock /app/
RUN gem install rack -v 2.0.5 && gem uninstall rack -v 2.0.6 && bundle install

ADD . .
CMD ["puma"]
