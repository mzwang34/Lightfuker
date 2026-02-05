/*
	Stellar by HTML5 UP
	html5up.net | @ajlkn
	Free for personal and commercial use under the CCA 3.0 license (html5up.net/license)
*/

(function($) {

	var	$window = $(window),
		$body = $('body'),
		$main = $('#main');

	// Breakpoints.
		breakpoints({
			xlarge:   [ '1281px',  '1680px' ],
			large:    [ '981px',   '1280px' ],
			medium:   [ '737px',   '980px'  ],
			small:    [ '481px',   '736px'  ],
			xsmall:   [ '361px',   '480px'  ],
			xxsmall:  [ null,      '360px'  ]
		});

	// Play initial animations on page load.
		$window.on('load', function() {
			window.setTimeout(function() {
				$body.removeClass('is-preload');
			}, 100);
		});

	// Nav.
		var $nav = $('#nav');

		if ($nav.length > 0) {

			// Shrink effect.
				$main
					.scrollex({
						mode: 'top',
						enter: function() {
							$nav.addClass('alt');
						},
						leave: function() {
							$nav.removeClass('alt');
						},
					});

			// Links.
				var $nav_a = $nav.find('a');

				$nav_a
					.scrolly({
						speed: 1000,
						offset: function() { return $nav.height(); }
					})
					.on('click', function() {

						var $this = $(this);

						// External link? Bail.
							if ($this.attr('href').charAt(0) != '#')
								return;

						// Deactivate all links.
							$nav_a
								.removeClass('active')
								.removeClass('active-locked');

						// Activate link *and* lock it (so Scrollex doesn't try to activate other links as we're scrolling to this one's section).
							$this
								.addClass('active')
								.addClass('active-locked');

					})
					.each(function() {

						var	$this = $(this),
							id = $this.attr('href'),
							$section = $(id);

						// No section for this link? Bail.
							if ($section.length < 1)
								return;

						// Scrollex.
							$section.scrollex({
								mode: 'middle',
								initialize: function() {

									// Deactivate section.
										if (browser.canUse('transition'))
											$section.addClass('inactive');

								},
								enter: function() {

									// Activate section.
										$section.removeClass('inactive');

									// No locked links? Deactivate all links and activate this section's one.
										if ($nav_a.filter('.active-locked').length == 0) {

											$nav_a.removeClass('active');
											$this.addClass('active');

										}

									// Otherwise, if this section's link is the one that's locked, unlock it.
										else if ($this.hasClass('active-locked'))
											$this.removeClass('active-locked');

								}
							});

					});

		}

	// Scrolly.
		$('.scrolly').scrolly({
			speed: 1000
		});
	
	// var $sliders = $('.comparison-container .slider');

	// if ($sliders.length > 0) {
		
	// 	$sliders.on('input', function() {
	// 		var $this = $(this);
	// 		var val = $this.val();
	// 		var $container = $this.closest('.comparison-container');

	// 		$container.find('.img-front-container').css('width', val + '%');
	// 		$container.find('.slider-button').css('left', val + '%');
	// 	});
	// 	var resizeFrontImages = function() {
	// 		$('.comparison-container').each(function() {
	// 			var $container = $(this);
	// 			var width = $container.width();
	// 			$container.find('.img-front').css('width', width + 'px');
	// 		});
	// 	};

	// 	$window.on('load resize', function() {
	// 		resizeFrontImages();
	// 	});
	// }

	var initComparisonSliders = function() {
		$('.comparison-container:not(.three-way) .slider').on('input', function() {
			var val = $(this).val();
			var $container = $(this).closest('.comparison-container');
			$container.find('.img-front-container').css('width', val + '%');
			$container.find('.slider-button').css('left', val + '%');
		});

		$('.comparison-container.three-way').each(function() {
			var $container = $(this);
			var $slider1 = $container.find('.slider-1'); 
			var $slider2 = $container.find('.slider-2'); 
			var $wrapper1 = $container.find('.img-wrapper-1'); 
			var $wrapper2 = $container.find('.img-wrapper-2'); 
			var $btn1 = $container.find('.button-1');
			var $btn2 = $container.find('.button-2');

			$slider1.on('input', function() {
				var val1 = parseFloat($(this).val());
				var val2 = parseFloat($slider2.val());

				if (val1 > val2) {
					val2 = val1;
					$slider2.val(val2);
					$wrapper2.css('width', val2 + '%');
					$btn2.css('left', val2 + '%');
				}

				$wrapper1.css('width', val1 + '%');
				$btn1.css('left', val1 + '%');
			});

			$slider2.on('input', function() {
				var val2 = parseFloat($(this).val());
				var val1 = parseFloat($slider1.val());

				if (val2 < val1) {
					val1 = val2;
					$slider1.val(val1); 
					$wrapper1.css('width', val1 + '%');
					$btn1.css('left', val1 + '%');
				}

				$wrapper2.css('width', val2 + '%');
				$btn2.css('left', val2 + '%');
			});
		});

		var adjustImageWidth = function() {
			$('.comparison-container').each(function() {
				var width = $(this).width();
				$(this).find('img').css('width', width + 'px');
			});
		};

		$(window).on('load resize', adjustImageWidth);
		adjustImageWidth();
	};

	if ($('.comparison-container').length > 0) {
		initComparisonSliders();
	}

})(jQuery);