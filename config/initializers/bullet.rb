unless Rails.env.production?
  Rails.application.config.after_initialize do
    Bullet.enable = true
    Bullet.alert = true
    Bullet.rails_logger = true
  end
end
