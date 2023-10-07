FROM node:18
WORKDIR /docker/cn_microservice
COPY ./ /docker/cn_microservice
RUN npm install
EXPOSE 8081
CMD [ "node", "app.js" ]
