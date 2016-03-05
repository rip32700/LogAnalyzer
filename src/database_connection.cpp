#include "database_connection.hpp"

std::string dateToString(const std::time_t time);
std::time_t stringToDate(const std::string timeStampString);
mongo::BSONObj createBSON(Device data);
mongo::BSONObj createBSON(Login data);
mongo::BSONObj createBSON(User data);

std::vector<std::string> Database_Connection::GetLogContent()
{
	std::vector<mongo::BSONObj> allLogs = getAll("hsp.logs");
	std::vector<std::string> retVector;

	for (auto log : allLogs) {
	    retVector.push_back(log.getStringField("log"));
	}

	return retVector;
}

std::vector<Login> Database_Connection::GetLogins()
{
	std::vector<mongo::BSONObj> allLogins = getAll("hsp.logins");
	std::vector<Login> retVector;

	for (auto login : allLogins) {
	    Login l(login.getStringField("loginname"), login.getStringField("mac"), stringToDate(login.getStringField("timestamp")));
	    retVector.push_back(l);
	}

	return retVector;
}

std::vector<User> Database_Connection::GetUsers()
{
	std::vector<mongo::BSONObj> allUsers = getAll("hsp.users");
	std::vector<User> retVector;

	for (auto user : allUsers) {
	    std::vector<DeviceLogin> logins;
	    for(auto deviceLogin : user.getField("logins").Array()) {
		DeviceLogin d(deviceLogin.Obj().getStringField("mac"), stringToDate(deviceLogin.Obj().getStringField("timestamp")));
		logins.push_back(d);
	    }
	    User u(user.getStringField("loginname"), logins);
	    retVector.push_back(u);
	}

	return retVector;
}

User Database_Connection::GetUserIfExistsOrInsert(std::string loginName, DeviceLogin currentLogin)
{
	mongo::BSONObj parameter = mongo::BSONObjBuilder().append("loginname", loginName).obj();
    	mongo::Query q = mongo::Query("{}").where("this.loginname == loginname", parameter); 
	mongo::BSONObj userBson = dbClient->findOne("hsp.users", q);
	std::vector<DeviceLogin> logins;

	if (userBson.isEmpty()) {
		logins.push_back(currentLogin);
		User newUser(loginName, logins);
	   	Upsert(newUser);

		return newUser;
	}

	for(auto deviceLogin : userBson.getField("logins").Array()) {
		DeviceLogin d(deviceLogin.Obj().getStringField("mac"), stringToDate(deviceLogin.Obj().getStringField("timestamp")));
		logins.push_back(d);
	}

	User user(userBson.getStringField("loginname"), logins);
	return user;
}

void Database_Connection::ReadFromFile(std::string fileName)
{
	std::ifstream infile(fileName);
	std::string line;
	std::vector<mongo::BSONObj> allLogLines;

	while (std::getline(infile, line))
	{
	    mongo::BSONObj logLine = mongo::BSONObjBuilder().append("log", line).obj();
	    allLogLines.push_back(logLine);
	}
	
	dbClient->insert("hsp.logs", allLogLines);
}

void Database_Connection::DropCollections(std::vector<std::string> collectionNames)
{
	for (auto collectionName : collectionNames) {
		std::cout << "Dropping " << collectionName << std::endl;
		dbClient->dropCollection(collectionName);
	} 
}

void Database_Connection::Upsert(Login value)
{
	mongo::BSONObj login = createBSON(value); 
    	mongo::Query q = mongo::Query("{}").where("this.loginname == loginname && this.mac == mac && this.timestamp == timestamp", login);

	dbClient->update("hsp.logins", q, login, true);
}

void Database_Connection::Upsert(std::vector<Login> values)
{
	for (Login l : values) {
		Upsert(l);
	}
}

void Database_Connection::Upsert(Device value)
{
	mongo::BSONObj device = createBSON(value); 
    	mongo::Query q = mongo::Query("{}").where("this.mac == mac", device);

	dbClient->update("hsp.devices", q, device, true);
}

void Database_Connection::Upsert(std::vector<Device> values)
{
	for (Device d : values) {
		Upsert(d);
	}
}

void Database_Connection::Upsert(User value)
{
	mongo::BSONObj user = createBSON(value); 
    	mongo::Query q = mongo::Query("{}").where("this.loginname == loginname", user);

	dbClient->update("hsp.users", q, user, true);
}

void Database_Connection::Upsert(std::vector<User> values)
{
	for (User u : values) {
		Upsert(u);
	}
}

mongo::BSONObj createBSON(Login data)
{
	return mongo::BSONObjBuilder().append("loginname", data.getLoginName())
				      .append("mac", data.getMac())
			 	      .append("timestamp",  dateToString(data.getTimeStamp()))
			       	      .obj();
}

mongo::BSONObj createBSON(User data)
{
	std::vector<mongo::BSONObj> logins;
	for (auto login : data.getLogins()) {
		logins.push_back(mongo::BSONObjBuilder().append("mac", login.getMac())
						 	.append("timestamp",  dateToString(login.getTimeStamp()))
						 	.obj());
	}

	return mongo::BSONObjBuilder().append("loginname", data.getLoginName())
			       	      .append("handymac", data.getHandyMac())
			       	      .append("logins", logins)
			              .obj();
}

mongo::BSONObj createBSON(Device data)
{
	return mongo::BSONObjBuilder().append("mac", data.getMac())
			       	      .append("logindate", data.getLoginDates())
			       	      .obj();
}

std::string dateToString(const std::time_t time) {
	char buffer[80];
	tm* ltm = localtime(&time);
	strftime(buffer,80,"%m/%d/%Y - %H:%M:%S",ltm);
	std::string formatedDate(buffer);
	return formatedDate;
}

std::time_t stringToDate(const std::string timeStampString) {
      struct tm tm;
      strptime(timeStampString.c_str(), "%m/%d/%Y - %H:%M:%S", &tm);
      return mktime(&tm);
}
