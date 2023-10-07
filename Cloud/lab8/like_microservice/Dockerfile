FROM node:18
WORKDIR /docker/like_microservice
COPY ./ /docker/like_microservice
RUN npm install
EXPOSE 8081
CMD [ "node", "app.js" ]
