Rails.application.routes.draw do
  post  '/captures', to: 'captures#create'

  resources :captures
  # For details on the DSL available within this file, see http://guides.rubyonrails.org/routing.html
end
