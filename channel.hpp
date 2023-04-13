#ifndef CHANNEL_HPP
# define CHANNEL_HPP

class  channel{
  private:
    map<name, client *> channel_users;
    client *admin;
    const std::string channel_name;
  public:
    channel(client *creator, const std::string &name);
    ~channel();
};

#endif
