FROM node:18
WORKDIR /docker/fb_microservice
COPY ./ /docker/fb_microservice
RUN npm install
EXPOSE 8082
CMD [ "node", "app.js" ]
