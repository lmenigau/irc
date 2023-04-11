#ifndef CHANNEL_HPP
# define CHANNEL_HPP

class  channel{
  private:
    vector<client *> users;
    client *admin;
    const std::string name;
  public:
    channel(client *creator, const std::string &name);
    ~channel();
};

#endif
