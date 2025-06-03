from django.core.management.base import BaseCommand
from custom_app.models import ColorBundle
from custom_app.models import Color


class Command(BaseCommand):
    help = "Seed the shop with items"

    def handle(self, *args, **kwargs):
        bundles = [
            {
                "bundle_id": 1,
                "rarity": 1,
                "name": "Basic bundle",
                "price": 10,
            },
            {
                "bundle_id": 2,
                "rarity": 1,
                "name": "Basic 2 bundle",
                "price": 10,
            },
            {
                "bundle_id": 3,
                "rarity": 1,
                "name": "Red bundle",
                "price": 15,
            },
            {
                "bundle_id": 4,
                "rarity": 1,
                "name": "Green bundle",
                "price": 15,
            },
            {
                "bundle_id": 5,
                "rarity": 1,
                "name": "Blue bundle",
                "price": 15,
            },
        ]
        colors = [
            {
                "color_id": 1,
                "bundle_id": 1,
                "rarity": 1,
                "r": 255,
                "g": 0,
                "b": 0,
            },
            {
                "color_id": 2,
                "bundle_id": 1,
                "rarity": 1,
                "r": 0,
                "g": 255,
                "b": 0,
            },
            {
                "color_id": 3,
                "bundle_id": 1,
                "rarity": 1,
                "r": 0,
                "g": 0,
                "b": 255,
            },
            {
                "color_id": 4,
                "bundle_id": 1,
                "rarity": 1,
                "r": 255,
                "g": 0,
                "b": 255,
            },
            {
                "color_id": 5,
                "bundle_id": 1,
                "rarity": 1,
                "r": 255,
                "g": 255,
                "b": 0,
            },
            {
                "color_id": 6,
                "bundle_id": 1,
                "rarity": 1,
                "r": 0,
                "g": 255,
                "b": 255,
            },

            {
                "color_id": 27,
                "bundle_id": 2,
                "rarity": 1,
                "r": 255,
                "g": 0,
                "b": 128,
            },
            {
                "color_id": 22,
                "bundle_id": 2,
                "rarity": 1,
                "r": 0,
                "g": 128,
                "b": 255,
            },
            {
                "color_id": 23,
                "bundle_id": 2,
                "rarity": 1,
                "r": 128,
                "g": 0,
                "b": 255,
            },
            {
                "color_id": 24,
                "bundle_id": 2,
                "rarity": 1,
                "r": 255,
                "g": 128,
                "b": 0,
            },
            {
                "color_id": 25,
                "bundle_id": 2,
                "rarity": 1,
                "r": 128,
                "g": 255,
                "b": 0,
            },
            {
                "color_id": 26,
                "bundle_id": 2,
                "rarity": 1,
                "r": 0,
                "g": 255,
                "b": 128,
            },

            {
                "color_id": 7,
                "bundle_id": 3,
                "rarity": 1,

                "r": 64,
                "g": 0,
                "b": 0,
            },
            {
                "color_id": 8,
                "bundle_id": 3,
                "rarity": 1,
                "r": 128,
                "g": 0,
                "b": 0,

            },
            {
                "color_id": 9,
                "bundle_id": 3,
                "rarity": 1,
                "r": 192,
                "g": 0,
                "b": 0,
            },
            {
                "color_id": 10,
                "bundle_id": 3,
                "rarity": 1,
                "r": 255,
                "g": 64,
                "b": 64,
            },
            {
                "color_id": 11,
                "bundle_id": 3,
                "rarity": 1,
                "r": 255,
                "g": 128,
                "b": 128,
            },

            {
                "color_id": 12,
                "bundle_id": 4,
                "rarity": 1,

                "r": 0,
                "g": 64,
                "b": 0,
            },
            {
                "color_id": 13,
                "bundle_id": 4,
                "rarity": 1,
                "r": 0,
                "g": 128,
                "b": 0,
            },
            {
                "color_id": 14,
                "bundle_id": 4,
                "rarity": 1,
                "r": 0,
                "g": 192,
                "b": 0,
            },
            {
                "color_id": 15,
                "bundle_id": 4,
                "rarity": 1,
                "r": 64,
                "g": 255,
                "b": 64,
            },
            {
                "color_id": 16,
                "bundle_id": 4,
                "rarity": 1,
                "r": 128,
                "g": 255,
                "b": 128,
            },

            {
                "color_id": 17,
                "bundle_id": 5,
                "rarity": 1,

                "r": 0,
                "g": 0,
                "b": 64,
            },
            {
                "color_id": 18,
                "bundle_id": 5,
                "rarity": 1,
                "r": 0,
                "g": 0,
                "b": 128,
            },
            {
                "color_id": 19,
                "bundle_id": 5,
                "rarity": 1,
                "r": 0,
                "g": 0,
                "b": 192,
            },
            {
                "color_id": 20,
                "bundle_id": 5,
                "rarity": 1,
                "r": 64,
                "g": 64,
                "b": 255,
            },
            {
                "color_id": 21,
                "bundle_id": 5,
                "rarity": 1,
                "r": 128,
                "g": 128,
                "b": 255,
            }
        ]

        # ----- HANDLE BUNDLES -----
        existing_bundle_ids = set(ColorBundle.objects.values_list("bundle_id", flat=True))
        input_bundle_ids = set(bundle["bundle_id"] for bundle in bundles)

        # Delete removed bundles
        bundles_to_delete = existing_bundle_ids - input_bundle_ids
        ColorBundle.objects.filter(bundle_id__in=bundles_to_delete).delete()
        for bundle_id in bundles_to_delete:
            self.stdout.write(self.style.WARNING(f"Deleted bundle: #{bundle_id}"))

        # Upsert bundles
        for bundle in bundles:
            obj, created = ColorBundle.objects.get_or_create(bundle_id=bundle["bundle_id"], defaults=bundle)
            if not created:
                for key, value in bundle.items():
                    setattr(obj, key, value)
                obj.save()
                self.stdout.write(self.style.SUCCESS(f"Updated bundle: #{bundle['bundle_id']} {bundle['name']}"))
            else:
                self.stdout.write(self.style.SUCCESS(f"Created bundle: #{bundle['bundle_id']} {bundle['name']}"))

        # ----- HANDLE COLORS -----
        existing_color_ids = set(Color.objects.values_list("color_id", flat=True))
        input_color_ids = set(color["color_id"] for color in colors)

        # Delete removed colors
        colors_to_delete = existing_color_ids - input_color_ids
        Color.objects.filter(color_id__in=colors_to_delete).delete()
        for color_id in colors_to_delete:
            self.stdout.write(self.style.WARNING(f"Deleted color: #{color_id}"))

        # Upsert colors
        for color in colors:
            obj, created = Color.objects.get_or_create(color_id=color["color_id"], defaults=color)
            if not created:
                for key, value in color.items():
                    if key == "bundle_id":
                        obj.bundle_id = value
                    else:
                        setattr(obj, key, value)
                obj.save()
                self.stdout.write(self.style.SUCCESS(f"Updated color: #{color['color_id']}"))
            else:
                self.stdout.write(self.style.SUCCESS(f"Created color: #{color['color_id']}"))
